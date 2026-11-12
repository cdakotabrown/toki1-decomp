#!/usr/bin/env python3
"""CI compile check: verify every matched C file compiles.

Runs WITHOUT the baserom. Verifies:
1. Every matched IDO C file compiles with IDO 5.2
2. Every matched GCC C file compiles with the appropriate Psy-Q GCC version
3. Reports instruction count vs declared size

PostProc steps are NOT run (many require the baserom for target comparison).
Byte-perfect verification requires running `build_level3_c.py` with the baserom.
"""
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
from parse_postproc import parse_compile_flags, parse_compiler, parse_compile_env

IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
GCCWRAP = ROOT / "tools" / "gcc" / "gccwrap.py"


def get_function_size(c_file):
    """Parse declared function size from C file header."""
    with open(c_file) as f:
        content = f.read(5000)
    m = re.search(r'Size:\s*(\d+)\s*bytes', content)
    return int(m.group(1)) if m else None


def get_text_size(obj_file):
    """Get .text section size from ELF object."""
    with open(obj_file, 'rb') as f:
        data = f.read()
    if len(data) < 0x34:
        return None
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    if e_shstrndx >= e_shnum:
        return None
    shstr_sh = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_sh + 0x10:shstr_sh + 0x14])[0]
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_off = struct.unpack('<I', data[sh:sh+4])[0]
        j = shstr_off + name_off
        name = b''
        while j < len(data) and data[j] != 0:
            name += bytes([data[j]])
            j += 1
        if name == b'.text':
            return struct.unpack('<I', data[sh+0x14:sh+0x18])[0]
    return None



_GCCWRAP_OPTS = None


def gccwrap_options():
    """Every --long option tools/gcc/gccwrap.py's argparse accepts."""
    global _GCCWRAP_OPTS
    if _GCCWRAP_OPTS is None:
        src = Path(GCCWRAP).read_text()
        _GCCWRAP_OPTS = set(re.findall(r'add_argument\(\s*(?:"-\w"\s*,\s*)?"(--[\w-]+)"', src))
        _GCCWRAP_OPTS -= {'--output', '--extra-flags'}   # never taken from @CompileFlags
    return _GCCWRAP_OPTS


def compile_gcc(c_file, out_o, gcc_version, extra_flags=None):
    """Compile with Psy-Q GCC via gccwrap.py."""
    version_num = gcc_version.replace('gcc', '')
    cmd = ['python3', str(GCCWRAP), str(c_file), '-o', str(out_o),
           '--include-dir', str(ROOT / 'include')]
    if gcc_version in ('ido52-aspsx', 'ido52-gas'):
        # the hybrid: IDO 5.2 codegen assembled PSY-Q style through gccwrap
        cmd += ['--codegen', 'ido52',
                '--assembler', 'gas' if gcc_version == 'ido52-gas' else 'maspsx']
    else:
        cmd += ['--gcc-version', version_num]

    # Separate gccwrap flags from cc1 flags.  The gccwrap/maspsx-side option
    # set is READ FROM tools/gcc/gccwrap.py's argparse definitions, so a new
    # gccwrap flag can never be forwarded to cc1 by mistake (2026-11-02: a
    # hand-kept copy here lacked --konami-lib-sched / --loc-load-nop, and 64
    # matched files failed CI with "cc1: Invalid option").
    GCCWRAP_FLAGS = gccwrap_options()
    cc1_flags = []
    if extra_flags:
        for flag in extra_flags:
            # the valued gccwrap flags, as in build_level3_c.py
            if flag.split('=', 1)[0] in GCCWRAP_FLAGS:
                cmd.append(flag)
            else:
                cc1_flags.append(flag)
        if cc1_flags:
            # Use = syntax so argparse doesn't treat -O1 as a flag
            cmd.append('--extra-flags=' + ' '.join(cc1_flags))

    result = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    return result.returncode == 0, result.stderr


def check_function(c_file):
    """Compile a matched C file. Returns (name, status, detail)."""
    name = c_file.stem
    compiler = parse_compiler(c_file)
    extra_flags = parse_compile_flags(c_file)
    env_vars = parse_compile_env(c_file)
    declared_size = get_function_size(c_file)

    with tempfile.TemporaryDirectory() as tmpdir:
        tmpdir = Path(tmpdir)
        raw_o = tmpdir / "raw.o"

        if compiler.startswith('gcc') or compiler in ('ido52-aspsx', 'ido52-gas'):
            ok, err = compile_gcc(c_file, raw_o, compiler, extra_flags)
            compiler_label = compiler.upper()
        else:
            cmd = [str(IDO_CC), '-O2', '-G0', '-mips1', '-EL', '-non_shared',
                   '-nostdinc', '-I', str(ROOT / 'include'),
                   '-I', str(ROOT / 'include' / 'psyq'),
                   '-c', str(c_file), '-o', str(raw_o)]
            if extra_flags:
                cmd.extend(extra_flags)

            env = None
            if env_vars:
                env = os.environ.copy()
                env.update(env_vars)

            try:
                result = subprocess.run(cmd, capture_output=True, text=True,
                                        timeout=120, env=env)
                ok = result.returncode == 0
                err = result.stderr
            except subprocess.TimeoutExpired:
                return name, 'FAIL', 'Compile timeout'

            compiler_label = 'IDO'

        if not ok:
            err_line = err.strip().split('\n')[-1][:150] if err else 'unknown error'
            return name, 'FAIL', f'{compiler_label} compile error: {err_line}'

        if not raw_o.exists():
            return name, 'FAIL', f'{compiler_label}: no output file'

        text_size = get_text_size(raw_o)
        if text_size is None:
            return name, 'FAIL', f'{compiler_label}: no .text section'

        detail = f'{compiler_label} {text_size}B'
        if declared_size:
            detail += f' (declared {declared_size}B)'

        return name, 'PASS', detail


def main():
    matched_dir = ROOT / "src" / "main" / "matched"
    semi_dir = ROOT / "src" / "main" / "semi-matched"
    matched_files = sorted(matched_dir.glob("*.c"))
    semi_files = sorted(semi_dir.glob("*.c")) if semi_dir.exists() else []

    if not matched_files:
        print("ERROR: No matched C files found!")
        sys.exit(1)

    # gccwrap.py assembles every Psy-Q GCC / ido52-gas / ido52-aspsx file
    # with the mipsel GNU assembler.  Without it ~200 files each fail with a
    # FileNotFoundError, which reads like a compile regression: say what is
    # missing once instead (the CI job installs binutils-mipsel-linux-gnu).
    if not shutil.which("mipsel-linux-gnu-as"):
        print("ERROR: mipsel-linux-gnu-as not found on PATH: install "
              "binutils-mipsel-linux-gnu (docs/GETTING-STARTED.md).")
        sys.exit(1)

    # matched/ failures fail CI; semi-matched/ failures are warnings only
    # (those files contain PC-port instrumentation that may not compile
    # cleanly under IDO 5.2 cfe — intentional).
    print(f"Compiling {len(matched_files)} matched + {len(semi_files)} semi-matched C files...")

    matched_set = set(matched_files)
    results = {'PASS': [], 'FAIL_MATCHED': [], 'FAIL_SEMI': []}
    all_files = matched_files + semi_files

    with ThreadPoolExecutor(max_workers=4) as executor:
        futures = {executor.submit(check_function, f): f for f in all_files}
        for i, future in enumerate(as_completed(futures), 1):
            name, status, detail = future.result()
            f = futures[future]
            if status == 'PASS':
                results['PASS'].append((name, detail))
                marker = '.'
            else:
                bucket = 'FAIL_MATCHED' if f in matched_set else 'FAIL_SEMI'
                results[bucket].append((name, detail))
                marker = 'F' if bucket == 'FAIL_MATCHED' else 'w'
            print(marker, end='', flush=True)
            if i % 80 == 0:
                print()

    print(f"\n\nResults:")
    print(f"  PASS:                     {len(results['PASS'])}")
    print(f"  FAIL (matched, hard):     {len(results['FAIL_MATCHED'])}")
    print(f"  FAIL (semi-matched, warn): {len(results['FAIL_SEMI'])}")

    if results['FAIL_MATCHED']:
        print(f"\nMATCHED FAILURES (CI-blocking):")
        for name, detail in sorted(results['FAIL_MATCHED']):
            print(f"  {name}: {detail}")

    if results['FAIL_SEMI']:
        print(f"\nSEMI-MATCHED warnings (PC-port instrumentation, non-blocking):")
        for name, detail in sorted(results['FAIL_SEMI']):
            print(f"  {name}: {detail}")

    if results['FAIL_MATCHED']:
        print(f"\n{len(results['FAIL_MATCHED'])} matched file(s) failed to compile!")
        sys.exit(1)

    print(f"\nAll {len(matched_files)} matched files compiled successfully.")


if __name__ == '__main__':
    main()
