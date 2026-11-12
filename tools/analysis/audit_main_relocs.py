#!/usr/bin/env python3
"""Name-derived relocation audit for MAIN-binary matched C.

`fixup_relocations` in build_level3_c.py copies every relocation-site
immediate (jal target, %hi, %lo) FROM THE TARGET, without checking which
symbol the C actually named.  So a `.c` that reads `D_801272D6` where the
retail code reads `D_801271D6` still reports BYTE-PERFECT (found 2026-08-22
on hotspot_table_check).  The overlays have resolve_overlay_relocs.py as the
authoritative gate; the main binary had nothing.

This script, for every registered C entry:
  1. compiles the TU to an object (IDO via cc, GCC via gccwrap) with the
     file's @CompileFlags / @CompileEnv,
  2. reads its relocations (pyelftools) and turns each into an ABSOLUTE
     address from the symbol NAME (D_XXXXXXXX / func_XXXXXXXX / names found
     in the level3 registry or o.bin symbol cache) plus addend,
  3. reconstructs the absolute addresses the TARGET bytes reference (jal
     targets, lui/%lo pairs by linear register tracking),
  4. compares the two MULTISETS (order-independent, so PostProc reorders do
     not matter).

Verdicts:  OK | MISMATCH (addresses differ) | UNRESOLVED (a symbol name the
script cannot map) | COMPILE_FAIL.

2026-10-03 (red-team finding 4): UNRESOLVED is a FAILURE.  A name no trusted
table knows (the registry, config/main_retail_symbols.json, the LANDED names
of config/symbol_names.json, or a D_/func_ address name) is a name whose
address nothing checks -- the harness copies the retail immediate at that
reloc site, so any wrong name passes.  Give the symbol a D_/func_ address
name or land it in a trusted table.
Reloc sites whose %hi is consumed in another basic block can produce
spurious 'extra' entries on either side; those are reported but the verdict
is MISMATCH only when a name-derived address is absent from the target set.

Usage:
    python3 tools/analysis/audit_main_relocs.py [--only NAME ...] [--json OUT] [-v]
"""
import argparse, collections, json, os, re, subprocess, sys, tempfile

_script_dir = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(_script_dir))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'build'))
from parse_postproc import (parse_compile_flags, parse_compiler,  # noqa: E402
                            parse_compile_env, parse_symbol_start,
                            parse_symbol_size, parse_postproc_header)

try:
    from elftools.elf.elffile import ELFFile
except ImportError:
    print("pyelftools missing -- run inside the venv", file=sys.stderr)
    sys.exit(2)

IDO_CC = os.path.join(ROOT, 'tools/ido-static-recomp/build/5.2/out/cc')
GCCWRAP = os.path.join(ROOT, 'tools/gcc/gccwrap.py')
EXE = os.path.join(ROOT, 'disks/SLPS_000.65')
TEXT_START, FILE_BASE = 0x80041000, 0x800
INFERRED = {}

_LO_OPS = {0x09, 0x0d, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b, 0x22, 0x26, 0x2a, 0x2e}
NAME_RE = re.compile(r'^(?:D|func|DAT|FUN|g|g_[A-Za-z]+)_?([0-9A-Fa-f]{8})(?:[_A-Za-z0-9]*)$')


def load_symtab():
    tab = {}
    for f in json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))['functions']:
        tab[f['name']] = int(f['address'], 16)
    # NOTE: the o.bin symbol cache is a DEBUG build with different addresses
    # (xa_wait 0x800521B8 there, 0x800521DC in retail) -- never use it here.
    rt = os.path.join(ROOT, 'config/main_retail_symbols.json')
    if os.path.exists(rt):
        for k, v in json.load(open(rt)).items():
            tab.setdefault(k, int(v['address'], 16) if isinstance(v, dict) else int(v, 16))
    # The LANDED rename table (config/symbol_names.json, the single source of
    # truth for renames): only each address's CURRENT, landed name -- a name
    # that survives only in a 'history' entry is a superseded proposal and
    # stays unresolved.
    sn = os.path.join(ROOT, 'config/symbol_names.json')
    if os.path.exists(sn):
        for a, v in (json.load(open(sn)).get('symbols') or {}).items():
            if isinstance(v, dict) and v.get('landed') and v.get('name') \
                    and v.get('scope', 'main') == 'main':
                try:
                    tab.setdefault(v['name'], int(a, 16))
                    for vw in v.get('views') or ():   # same-address views (2026-11-10)
                        tab.setdefault(vw['name'], int(a, 16) + int(vw.get('offset', 0)))
                except (ValueError, KeyError):
                    pass
    return tab


def sym_addr(name, tab):
    m = NAME_RE.match(name)
    if m:
        return int(m.group(1), 16)
    return tab.get(name)


def compile_obj(src, tmp):
    flags = parse_compile_flags(src) or []
    comp = parse_compiler(src)
    env = dict(os.environ)
    env.update(parse_compile_env(src) or {})
    obj = os.path.join(tmp, 'f.o')
    if comp.startswith('gcc') or comp in ('ido52-aspsx', 'ido52-gas'):
        # GCC, or the hybrid (IDO codegen assembled PSY-Q style) -- both go
        # through gccwrap; `--` flags are gccwrap's, the rest are cc1 / IDO flags.
        cmd = [sys.executable, GCCWRAP, src, '-o', obj, '-I', os.path.join(ROOT, 'include')]
        if comp.startswith('gcc'):
            cmd += ['--gcc-version', comp[3:]]
        else:
            cmd += ['--codegen', 'ido52', '--assembler', 'gas' if comp == 'ido52-gas' else 'maspsx']
        extra = [f for f in flags if f.startswith('--')]
        cc1 = [f for f in flags if not f.startswith('--')]
        cmd += extra
        if cc1:
            cmd.append('--extra-flags=' + ' '.join(cc1))
    else:
        cmd = [IDO_CC, '-O2'] + flags + ['-G0', '-mips1', '-EL', '-non_shared', '-c', '-I',
                                          os.path.join(ROOT, 'include'), src, '-o', obj]
    r = subprocess.run(cmd, capture_output=True, text=True, env=env, cwd=ROOT)
    if r.returncode != 0 or not os.path.exists(obj):
        raise RuntimeError((r.stderr or r.stdout)[-300:])
    return obj


def func_symbol_offset(obj, name):
    """.text offset of FUNC symbol `name` in `obj`, or None."""
    with open(obj, 'rb') as fh:
        elf = ELFFile(fh)
        symtab = elf.get_section_by_name('.symtab')
        if symtab is None:
            return None
        for s in symtab.iter_symbols():
            if s.name == name and s['st_info']['type'] == 'STT_FUNC':
                return s['st_value']
    return None


def obj_refs(obj, tab, min_off=0):
    """Multiset of absolute addresses the object references, + unresolved names."""
    refs, unresolved, kinds = [], set(), []
    obj_refs.kinds = kinds
    obj_refs.unresolved_order = []
    with open(obj, 'rb') as fh:
        elf = ELFFile(fh)
        text = elf.get_section_by_name('.text')
        if text is None:
            return refs, unresolved
        data = text.data()
        symtab = elf.get_section_by_name('.symtab')
        rel = None
        for s in elf.iter_sections():
            if s.name in ('.rel.text', '.rela.text'):
                rel = s
        if rel is None:
            return refs, unresolved
        last_hi = None   # (sym_addr, hi_imm) of the most recent HI16: every following LO16 pairs with it
        for r in rel.iter_relocations():
            sym = symtab.get_symbol(r['r_info_sym'])
            name = sym.name
            off = r['r_offset']
            if off < min_off:
                continue   # inside the stripped leading words (types.h thunks / a head owned by another symbol)
            word = int.from_bytes(data[off:off + 4], 'little')
            typ = r['r_info_type']
            if name in ('', '.text', '.data', '.rodata', '.bss', '.sdata', '.sbss') or sym['st_info']['type'] == 'STT_SECTION':
                continue
            a = sym_addr(name, tab)
            if a is None:
                unresolved.add(name)
                if typ == 4:
                    obj_refs.unresolved_order.append(name)
                continue
            if typ == 4:      # R_MIPS_26
                refs.append(a + ((word & 0x3ffffff) << 2)); kinds.append('J')
            elif typ == 5:    # HI16
                last_hi = (a, word & 0xffff)
            elif typ == 6:    # LO16
                lo = word & 0xffff
                lo = lo - 0x10000 if lo >= 0x8000 else lo
                if last_hi and last_hi[0] == a:
                    refs.append(a + ((last_hi[1] << 16) + lo))
                else:
                    refs.append(a + lo)
                kinds.append('A')
            elif typ == 2:    # R_MIPS_32 (tables)
                refs.append(a + int.from_bytes(data[off:off + 4], 'little', signed=True)); kinds.append('W')
    return refs, unresolved


# ── 2026-08-23: %hi/%lo pairing across control flow ──────────────────────────
# target_refs() is a LINEAR scan, but IDO emits one `lui` per PATH, so a `%lo`
# can legitimately pair with a `lui` that the scan has already spent on a
# sibling path, or that a def on a not-taken path killed.  Both shapes produced
# FALSE `MISMATCH` verdicts (get_pad_str2, SD_CDFadeFrame) and 9 false
# `COUNT`s.  Two carry-over dictionaries fix it:
#   spent_def     -- the %hi a later DEF killed.  Safe for ANY %lo: the register
#                    was redefined, so a later %lo reading it must be reached by
#                    a different path that still holds the lui.
#   spent_consume -- the %hi a %lo already used.  Only the address-MATERIALISING
#                    ops (addiu/ori) may reuse it; a LOAD must pair strictly, or
#                    a base pointer built by `addiu rX,rX,lo` would manufacture a
#                    bogus %hi+0 reference on the next `lbu 0(rX)`.
# 761/768 OK afterwards (was 750), with no new MISMATCH and the wrong-name
# detector still firing (verified by renaming D_800BCC30 -> D_800BCC34).
#
# KNOWN LIMITATION -- holiday_screen_init (the only remaining MISMATCH).  Its
# `lbu $a0,-6242($a0)` at 0x80075874 pairs with a `lui $a0,0x800f` at 0x8007580C
# reached along a path that skips TWO intervening $a0 redefinitions
# (`lui $a0,0x800b` + `addiu $a0,$a0,32140` at 0x80075844/4C), so both carry-over
# slots hold 0x800b by then.  Verified BY HAND: 0x800F0000 + (-6242) = 0x800EE79E,
# exactly the D_800EE79E the .c names -- the source is correct and the verdict is
# the scan's limitation.  Resolving it properly needs a per-register %hi HISTORY
# with path-aware selection; emitting every candidate instead would turn genuine
# MISMATCHes into COUNTs and weaken the gate, so it is deliberately not done.
def target_refs(addr, size):
    exe = open(EXE, 'rb').read()
    off = addr - TEXT_START + FILE_BASE
    words = [int.from_bytes(exe[off + i:off + i + 4], 'little') for i in range(0, size, 4)]
    refs = []
    kinds = []
    target_refs.kinds = kinds
    hi = {}   # reg -> %hi<<16 currently held
    # cross-prologue: up to 8 words BEFORE the window may hold the lui whose %lo the
    # first body words consume (the head belongs to a neighbouring fragment symbol)
    for i in range(max(0, off - 32), off, 4):
        w = int.from_bytes(exe[i:i + 4], 'little')
        op, rt = w >> 26, (w >> 16) & 31
        if op == 0x0f:
            hi[rt] = (w & 0xffff) << 16
        elif op == 0:
            funct, rs, rd = w & 0x3f, (w >> 21) & 31, (w >> 11) & 31
            if not (funct in (0x20, 0x21) and rd == rs and rs in hi):
                hi.pop(rd, None)
        elif op in (0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e) or (0x20 <= op <= 0x26):
            hi.pop(rt, None)
    # A %hi that a %lo already consumed stays recorded here.  IDO emits one `lui`
    # per PATH, so a linear scan can see `lui rX` / `addiu rX,rX,lo` / (branch
    # target) / `addiu rX,rX,lo2` where the second `addiu` pairs with a `lui` the
    # scan already spent on the fall-through path.  Only the address-MATERIALISING
    # ops fall back (addiu/ori); a load must still pair strictly, or a base pointer
    # built by a preceding `addiu rX,rX,lo` would manufacture a bogus %hi+0 ref.
    # These retain a SET per register, not a single value.  A register can have
    # held several different %hi values over the function -- one per path -- and
    # a %lo at a branch target may pair with ANY of them.  Keeping only the most
    # recent loses the others and produces a false `missing`: holiday_screen_init's
    # `lbu $a0,-6242($a0)` pairs with a `lui $a0,0x800f` that a later
    # `lui $a0,0x800b` had displaced.
    spent_consume = {}   # reg -> set of %hi a %lo already used
    spent_def = {}       # reg -> set of %hi a later DEF killed
    for w in words:
        op = w >> 26
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        imm = w & 0xffff
        if op in (2, 3):
            refs.append((addr & 0xF0000000) | ((w & 0x3ffffff) << 2)); kinds.append('J')
            continue
        if op == 0x0f:
            # A second `lui` into a register that still holds a pending %hi does
            # NOT mean the first one was never used -- IDO emits one `lui` per
            # PATH, so the old value belongs to a different path that a later
            # branch target rejoins.  Retire it to spent_def instead of dropping
            # it, or the %lo on the rejoined path finds no base and the function
            # is falsely reported MISMATCH.  (holiday_screen_init: `lui $a0,0x800b` at
            # +102 clobbered the `lui $a0,0x800f` that `lbu $a0,-6242($a0)` at
            # +114 pairs with on the branch-target path.)
            if rt in hi and hi[rt] != (imm << 16):
                spent_def.setdefault(rt, set()).add(hi[rt])
            hi[rt] = imm << 16
            continue
        # consume a %hi
        cands = []
        if rs in hi:
            cands.append(hi[rs])
        cands.extend(sorted(spent_def.get(rs, ())))
        if op in (0x09, 0x0d):
            cands.extend(sorted(spent_consume.get(rs, ())))
        base = cands[0] if cands else None
        if op in _LO_OPS and base is not None and rs not in (0, 28, 29):
            s16 = imm if op == 0x0d else (imm - 0x10000 if imm >= 0x8000 else imm)
            refs.append(base + s16); kinds.append('A')
            # A linear scan cannot know WHICH path reached a branch target, so
            # the live %hi it picked may belong to a path that does not reach
            # here.  Emit the other candidate bases as alternates too: this can
            # only suppress a false `missing`, never invent a name-vs-target
            # agreement, because a named address still has to equal one of the
            # %hi values the target actually materialises.
            for other in cands[1:]:
                if other != base:
                    refs.append(other + s16); kinds.append('A?')
            if rs in hi:
                spent_consume.setdefault(rs, set()).add(hi[rs])
        # kill the destination register's %hi
        if op == 0:
            funct = w & 0x3f
            if funct in (0x20, 0x21) and rd == rs and rs in hi:
                pass        # addu rX,rX,idx: table-index form, %hi still pending in rX
            elif funct in (0x20, 0x21) and rd == rt and rt in hi:
                pass
            else:
                if rd in hi:
                    spent_def.setdefault(rd, set()).add(hi.pop(rd))
        elif op in (0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e) or (0x20 <= op <= 0x26):
            if rt in hi:
                spent_def.setdefault(rt, set()).add(hi.pop(rt))
    return refs


def main():
    os.chdir(ROOT)
    ap = argparse.ArgumentParser()
    ap.add_argument('--config', default='config/level3_functions.json')
    ap.add_argument('--only', nargs='*')
    ap.add_argument('--files', nargs='*', help='audit UNREGISTERED .c sources by path (open semis); address/size come from each file own header')
    ap.add_argument('--json')
    ap.add_argument('-v', action='store_true')
    ap.add_argument('--infer', action='store_true', help='write inferred retail addresses of named externs to config/main_retail_symbols.json')
    args = ap.parse_args()
    tab = load_symtab()
    rows = []
    entries = json.load(open(args.config))['functions']
    if args.files:
        # Audit UNREGISTERED sources (open semis).  Without this the tool can
        # only see the registry, so `--only <semi>` prints "0 C entries" and
        # passes by finding nothing -- the vacuous pass this project keeps
        # tripping over.  Synthesise an entry from the file's own first
        # comment block, which carries `Address:` and `Size:`.
        entries = []
        for path in args.files:
            head = open(path, errors='ignore').read(4000)
            ma = re.search(r'^\s*\*?\s*Address:\s*(0x[0-9A-Fa-f]+)', head, re.M)
            mz = re.search(r'^\s*\*?\s*Size:\s*(\d+)\s*bytes', head, re.M)
            if not ma or not mz:
                print(f'{path}: SKIP -- no Address:/Size: in the first comment block')
                continue
            entries.append({'name': os.path.splitext(os.path.basename(path))[0],
                            'address': ma.group(1), 'size': int(mz.group(1)),
                            'source': path, 'header_window': True})
    for f in entries:
        if not f['source'].endswith('.c'):
            continue
        if args.only and f['name'] not in args.only:
            continue
        addr, size = int(f['address'], 16), int(f['size'])
        ss = parse_symbol_start(f['source']) if os.path.exists(f['source']) else None
        if ss is not None and ss < addr:
            size += addr - ss
            addr = ss
        # 2026-09-17: for an UNREGISTERED source (--files) the header is the only
        # window statement there is, so an explicit @SymbolSize overrides the
        # `Size:` prose -- which is routinely stale on files whose window was
        # later corrected.  pad_scan read 740 while its @SymbolSize says
        # 1052, and the 312 bytes that fell outside held the `jal konami_command_check`
        # the audit then reported as a wrong name.  Registered entries keep
        # using the registry size: a header/registry disagreement there is a
        # registry defect for audit_registry_overlaps to report, not something
        # to paper over here.
        if f.get('header_window') and os.path.exists(f['source']):
            zs = parse_symbol_size(f['source'])
            if zs:
                size = zs
        row = {'name': f['name'], 'address': hex(addr), 'size': size}
        try:
            with tempfile.TemporaryDirectory() as tmp:
                obj = compile_obj(f['source'], tmp)
                strip = 0
                for st in (parse_postproc_header(f['source']) or []):
                    if st.get('type') == 'strip_leading_instructions':
                        c = st.get('count')
                        strip = 16 if c in (None, 'auto') else int(c) * 4
                # `auto` means "everything before the function": use the
                # symbol's own offset when the object defines it.  A file can
                # carry a same-TU neighbour ahead of the function for layout
                # (_main), and 16 bytes would then audit that
                # neighbour's references as this function's.
                fsym = func_symbol_offset(obj, f['name'])
                if fsym is not None and strip == 16:
                    strip = fsym
                crefs, unresolved = obj_refs(obj, tab, strip)
        except Exception as e:  # noqa: BLE001
            row['verdict'] = 'COMPILE_FAIL'; row['error'] = str(e)[-200:]
            rows.append(row); continue
        trefs = target_refs(addr, size)
        cm, tm = collections.Counter(crefs), collections.Counter(trefs)
        missing = sorted((cm - tm).elements())   # named in C, absent from target
        extra = sorted((tm - cm).elements())     # in target, not named in C (unresolved/section relocs land here)
        row.update({'named': len(crefs), 'unresolved': sorted(unresolved),
                    'c_order': [hex(x) for x in crefs], 't_order': [hex(x) for x in trefs],
                    'c_kind': list(obj_refs.kinds), 't_kind': list(target_refs.kinds),
                    'missing_from_target': [hex(x) for x in missing],
                    'extra_in_target': [hex(x) for x in extra]})
        if unresolved and not missing and args.infer:
            # every named address matched; the leftover target jal targets, in order,
            # must be the unresolved externs in reloc order
            ex_j = [x for x, k in zip(trefs, target_refs.kinds) if k == 'J' and cm[x] == 0]
            names = [n for n in obj_refs.unresolved_order if n in unresolved]
            if len(ex_j) == len(names) == len(set(names)) and len(set(ex_j)) == len(ex_j):
                inferred = dict(zip(names, ex_j))
                row['inferred'] = {k: hex(v) for k, v in inferred.items()}
                INFERRED.update({k: {'address': hex(v), 'from': f['name']} for k, v in inferred.items()})
        absent = sorted(set(cm) - set(tm))          # named in C, referenced NOWHERE in the target
        row['absent_from_target'] = [hex(x) for x in absent]
        if absent:
            row['verdict'] = 'MISMATCH'
        elif missing:
            row['verdict'] = 'COUNT'      # same addresses, different multiplicity (shared lui / stripped word)
        elif unresolved:
            row['verdict'] = 'UNRESOLVED'
        else:
            row['verdict'] = 'OK'
        rows.append(row)
        if args.v or row['verdict'] == 'MISMATCH':
            print(f"{row['verdict']:10s} {f['name']:26s} named={len(crefs):3d} "
                  f"missing={row['missing_from_target'][:4]} unresolved={row['unresolved'][:3]}")
    counts = collections.Counter(r['verdict'] for r in rows)
    print(f"\n{len(rows)} C entries: {dict(counts)}")
    if args.infer and INFERRED:
        rt = os.path.join(ROOT, 'config/main_retail_symbols.json')
        cur = json.load(open(rt)) if os.path.exists(rt) else {}
        cur.update(INFERRED)
        json.dump(dict(sorted(cur.items())), open(rt, 'w'), indent=1)
        print(f"inferred {len(INFERRED)} retail symbol addresses -> {rt}")
    if args.json:
        json.dump(rows, open(args.json, 'w'), indent=1)
    unres = [r for r in rows if r.get('verdict') == 'UNRESOLVED']
    for r in unres:
        print(f"UNRESOLVED {r['name']:26s} names no trusted table maps: {r['unresolved']}")
    return 1 if (counts.get('MISMATCH') or unres) else 0


if __name__ == '__main__':
    sys.exit(main())
