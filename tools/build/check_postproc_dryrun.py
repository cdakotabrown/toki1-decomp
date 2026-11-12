#!/usr/bin/env python3
"""Dry-run every overlay function's PostProc pipeline to catch stale annotations.

For each src/overlay/*/func_*.c that has @PostProc directives:
  1. Compile the C with IDO 5.2 (no PostProc).
  2. Apply each PostProc step in order.
  3. Report any step that fails (e.g. patch_insn_reg_field with a stale
     FROM register, swap_instructions out of range, etc.).

Does NOT verify byte match against target — that requires the .EXN
files which aren't in the repo. This catches the class of regressions
where someone edits the C and the existing PostProc annotations no
longer apply cleanly.

Exit 0 if all PostProc applies cleanly. Exit 1 with a list of failing
files otherwise.
"""
import os
import re
import subprocess
import sys
import tempfile
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

# redirect_stdout swaps the process-global sys.stdout; without a lock a
# worker's redirect captures every other thread's prints (including the
# final FAILURES report), so CI exits 1 with no diagnostics.
_REDIRECT_LOCK = threading.Lock()

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'overlay'))
sys.path.insert(0, str(ROOT / 'tools' / 'build'))

from overlay_postproc import parse_postproc, apply_postproc, get_target_exn
from parse_postproc import parse_symbol_start_text

IDO_CC = ROOT / 'tools' / 'ido-static-recomp' / 'build' / '5.2' / 'out' / 'cc'
SRC_OVERLAY = ROOT / 'src' / 'overlay'
ASM_OVERLAY = ROOT / 'asm' / 'overlay'
CONFIG_OVERLAY = ROOT / 'config' / 'overlay'
STATUS_JSON = ROOT / 'config' / 'overlay_match_status.json'

# Steps that need the target .EXN binary (not in the repo — absent in CI).
# apply_postproc silently skips them when the EXN is missing, which leaves the
# object in compile order and makes every LATER step's offsets/registers
# meaningless — so validation must stop at the first such step.
EXN_DEPENDENT_STEPS = {'scheduler_normalizer'}

_sym_cache = {}


def sym_addr(overlay_name, fn):
    """Function address the way the real pipeline resolves it:
    @SymbolStart override > config/overlay/<OV>.sym > name-derived."""
    if overlay_name not in _sym_cache:
        funcs = {}
        sym_file = CONFIG_OVERLAY / f'{overlay_name}.sym'
        if sym_file.exists():
            for line in sym_file.read_text().splitlines():
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                parts = line.split(':')
                if len(parts) >= 3:
                    funcs[parts[2]] = int(parts[0], 16)
        _sym_cache[overlay_name] = funcs
    return _sym_cache[overlay_name].get(fn)


def load_status():
    try:
        import json
        d = json.loads(STATUS_JSON.read_text())
        out = {}
        for ov, fns in d.items():
            if not isinstance(fns, dict):
                continue
            for fn, e in fns.items():
                out[f'{ov}/{fn}'] = e.get('status', '?') if isinstance(e, dict) else str(e)
        return out
    except Exception:
        return {}


def has_postproc(c_file):
    try:
        with open(c_file) as f:
            return '@PostProc' in f.read(8000)
    except OSError:
        return False


def compile_one(c_file, obj):
    """Compile exactly as tools/overlay/test_overlay_match.py does: the shared
    flag line and per-file tail from tools/build/ido_flags.py (which applies the
    automatic -Wc,-pic1) and the anchored @CompileFlags / @CompileEnv parsers of
    parse_postproc.  (2026-11-02: a private copy here regex-scanned the header
    PROSE too -- `@CompileFlags: -Wc,-pic0` -- HONEST ... handed cc a `--` --
    and skipped the pic1 default, so 16 byte-perfect functions failed CI.)"""
    sys.path.insert(0, str(ROOT / 'tools' / 'overlay'))
    from parse_postproc import parse_compile_flags, parse_compile_env
    from ido_flags import base_flags, compile_flag_tail
    cmd = ([str(IDO_CC)] + base_flags([ROOT / 'include', ROOT / 'include' / 'psyq'])
           + compile_flag_tail(c_file, compile_flags=parse_compile_flags(c_file),
                               extra_cflags=[])
           + ['-c', str(c_file), '-o', str(obj)])
    env = os.environ.copy()
    env.update(parse_compile_env(c_file) or {})
    # Match test_overlay_match.py: isolate toolchain intermediates per
    # invocation -- IDO uses fixed-name temp files (cfe/ugen/uopt/as1) that
    # collide across concurrent compiles, corrupting codegen.
    env['TMPDIR'] = str(obj.parent)
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=60, env=env,
                       cwd=str(obj.parent))
    return r.returncode == 0, r.stderr


def func_addr_from_name(fn):
    m = re.match(r'func_([0-9a-fA-F]+)', fn)
    return int(m.group(1), 16) if m else 0


def check_function(c_file):
    overlay_name = c_file.parent.name
    fn = c_file.stem
    asm_file = ASM_OVERLAY / overlay_name / f'{fn}.s'
    # Address the way the pipeline resolves it — a name-derived address is
    # wrong for @SymbolStart/merged-window functions and shifts the EXN
    # window scheduler_normalizer aligns against (garbage downstream).
    addr = None
    try:
        head = c_file.read_text()[:5000]
        addr = parse_symbol_start_text(head)
    except OSError:
        pass
    if addr is None:
        addr = sym_addr(overlay_name, fn)
    if addr is None:
        addr = func_addr_from_name(fn)
    steps = parse_postproc(c_file)
    if not steps:
        return fn, 'SKIP', 'no postproc'
    # Without the target EXN (CI: assets aren't in the repo), EXN-dependent
    # steps are silently skipped by apply_postproc, so later steps see an
    # unscheduled object — validate only the prefix before the first one.
    if get_target_exn(overlay_name) is None:
        cut = next((i for i, s in enumerate(steps)
                    if s.get('type') in EXN_DEPENDENT_STEPS), None)
        if cut is not None:
            if cut == 0:
                return fn, 'SKIP', 'needs target EXN (not available)'
            steps = steps[:cut]
    with tempfile.TemporaryDirectory() as tmpdir:
        obj = Path(tmpdir) / f'{fn}.o'
        ok, err = compile_one(c_file, obj)
        if not ok:
            tail = (err or '').strip().splitlines()[-1][:160] if err else ''
            return fn, 'FAIL', f'compile: {tail}'
        # Capture stderr/stdout via stdout redirection — apply_postproc
        # writes diagnostics to stdout. Run with verbose to surface step
        # failures.
        import io, contextlib
        buf = io.StringIO()
        try:
            with _REDIRECT_LOCK, \
                 contextlib.redirect_stdout(buf), contextlib.redirect_stderr(buf):
                ok = apply_postproc(obj, steps, addr, overlay_name,
                                    verbose=False,
                                    target_asm=asm_file if asm_file.exists() else None)
        except SystemExit as e:
            # Some postproc tools sys.exit() on error; without this the
            # exit unwinds through the worker and kills the whole check.
            return fn, 'FAIL', f'postproc sys.exit({e.code}): {buf.getvalue().strip().splitlines()[-1][:160] if buf.getvalue().strip() else ""}'
        except Exception as e:
            return fn, 'FAIL', f'postproc exception: {e}'
        out = buf.getvalue()
        if 'FAILED' in out or 'Error:' in out:
            line = next((ln for ln in out.splitlines()
                         if 'FAILED' in ln or 'Error:' in ln), '')
            return fn, 'FAIL', line.strip()[:200]
        if not ok:
            return fn, 'FAIL', 'apply_postproc returned False'
        return fn, 'PASS', None


def main():
    candidates = []
    for ov_dir in sorted(SRC_OVERLAY.iterdir()):
        if not ov_dir.is_dir():
            continue
        for c_file in sorted(ov_dir.glob('*.c')):
            if has_postproc(c_file):
                candidates.append(c_file)

    print(f'Checking PostProc on {len(candidates)} overlay C files...')
    status_map = load_status()
    # Only functions that CLAIM a byte match gate CI: a stale directive on an
    # honest semi is cleanup work, not a build regression (the authoritative
    # pipeline ignores step failures entirely and judges final bytes — which
    # CI cannot do without the EXN assets).
    HARD_STATUSES = {'byte_perfect', 'reloc_only'}
    n_pass = 0
    n_skip = 0
    # IDO's toolchain hands off through basename-derived intermediates; under
    # sustained wide concurrency (this sweep compiles thousands of files) two
    # workers occasionally collide and produce a spuriously-failing object.
    # A stale directive fails DETERMINISTICALLY, so any real failure survives
    # a serial retry while a concurrency flake clears — collect the concurrent
    # failures, then re-verify each one alone before recording it.
    concurrent_fail = []
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
        futs = {ex.submit(check_function, c): c for c in candidates}
        done = 0
        for fut in as_completed(futs):
            c = futs[fut]
            try:
                fn, status, detail = fut.result()
            except (Exception, SystemExit) as e:
                fn = c.stem
                status = 'FAIL'
                detail = f'exception: {type(e).__name__}: {e}'
            done += 1
            if status == 'FAIL':
                concurrent_fail.append(c)
                print('?', end='', flush=True)
            elif status == 'SKIP':
                n_skip += 1
                print('s', end='', flush=True)
            elif status == 'PASS':
                n_pass += 1
                print('.', end='', flush=True)
            if done % 80 == 0:
                print(flush=True)
    print()

    hard_fails = []
    warn_fails = []
    if concurrent_fail:
        print(f'\nRe-verifying {len(concurrent_fail)} failure(s) serially '
              f'(filters concurrency flakes)...')
        for c in concurrent_fail:
            fn, status, detail = check_function(c)
            if status != 'FAIL':
                n_pass += 1 if status == 'PASS' else 0
                n_skip += 1 if status == 'SKIP' else 0
                print('+', end='', flush=True)  # flake cleared on retry
                continue
            key = f'{c.parent.name}/{fn}'
            rec = status_map.get(key, '?')
            if rec in HARD_STATUSES:
                hard_fails.append((key, rec, detail))
                print('F', end='', flush=True)
            else:
                warn_fails.append((key, rec, detail))
                print('w', end='', flush=True)
        print()
    print(f'\nResults: PASS={n_pass}  SKIP={n_skip}  '
          f'FAIL(match-claiming)={len(hard_fails)}  '
          f'stale-on-semi warnings={len(warn_fails)}\n')
    if warn_fails:
        print('WARNINGS (stale directives on non-matching functions — cleanup, not CI-gating):')
        for key, rec, detail in warn_fails[:40]:
            print(f'  {key} [{rec}]: {detail}')
        if len(warn_fails) > 40:
            print(f'  ... and {len(warn_fails) - 40} more')
        print()
    if hard_fails:
        print('FAILURES (functions recorded byte_perfect/reloc_only whose PostProc no longer applies):')
        for key, rec, detail in hard_fails[:80]:
            print(f'  {key} [{rec}]: {detail}')
        if len(hard_fails) > 80:
            print(f'  ... and {len(hard_fails) - 80} more')
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
