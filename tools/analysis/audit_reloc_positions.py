#!/usr/bin/env python3
"""POSITIONAL relocation audit for one main-binary C file (2026-09-05).

audit_main_relocs.py compares the MULTISET of name-derived addresses against
the target, so a swap_instructions chain that exchanges two `jal`s (or two
`%lo` words) passes it while the harness copies the target immediates and
prints BYTE-PERFECT.  This script compiles the file through the FULL
pipeline (all PostProc applied, object kept via L3_KEEP_OBJ), reads the
relocations of the final object at their BODY offsets, derives each symbol's
address from its NAME (+addend) and checks it against the retail word at the
same offset: jal target for R_MIPS_26, low half for R_MIPS_LO16, high half
(with the paired %lo carry) for R_MIPS_HI16.

    python3 tools/analysis/audit_reloc_positions.py FUNC [FUNC ...]
    python3 tools/analysis/audit_reloc_positions.py --registered-since <git-rev>
    python3 tools/analysis/audit_reloc_positions.py --all
Verdict per function: OK / MISMATCH (list) / UNRESOLVED names (a failure
since 2026-10-03: a name no trusted table maps is a name nothing checks).
"""
import os, sys, struct, json, re, tempfile, subprocess
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools/build')); sys.path.insert(0, os.path.join(ROOT, 'tools/analysis'))
from elftools.elf.elffile import ELFFile
import audit_main_relocs as amr

def find_source(fn):
    for e in json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))['functions']:
        if e['name'] == fn:
            return e['source'], int(e['address'], 16), e['size']
    for d in ('matched', 'semi-matched'):
        p = f'src/main/{d}/{fn}.c'
        if os.path.exists(p):
            s = open(p).read()
            m = re.search(r'@SymbolStart:\s*(0x[0-9A-Fa-f]+)', s) or re.search(r'Address:\s*(0x[0-9A-Fa-f]+)', s)
            z = re.search(r'@SymbolSize:\s*(\d+)', s) or re.search(r'Size:\s*(\d+)', s)
            return p, int(m.group(1), 16), int(z.group(1))
    return None

def audit(fn, tab, img):
    import build_level3_c as b, test_single_match as t
    src, addr, size = find_source(fn)
    pp = t.parse_postproc_header(src); fl = t.parse_compile_flags(src); comp = t.parse_compiler(src)
    saved = dict(os.environ)
    with tempfile.TemporaryDirectory() as td:
        objp = os.path.join(td, 'final.o')
        os.environ.update(t.parse_compile_env(src) or {}); os.environ['L3_KEEP_OBJ'] = objp
        try:
            b.compile_c_function(src, fn, addr, postproc_steps=pp, extra_flags=fl, compiler=comp)
        finally:
            os.environ.clear(); os.environ.update(saved)   # @CompileEnv must not leak into the next function
        if os.environ.get('ARP_KEEP'):                     # keep the final object for hand inspection
            import shutil; shutil.copy(objp, os.path.join(os.environ['ARP_KEEP'], fn + '.o'))
        elf = ELFFile(open(objp, 'rb'))
        text = elf.get_section_by_name('.text'); tdata = text.data()
        symtab = elf.get_section_by_name('.symtab')
        rel = elf.get_section_by_name('.rel.text') or elf.get_section_by_name('.rela.text')
        relocs = []
        if rel is not None:
            for r in rel.iter_relocations():
                sym = symtab.get_symbol(r['r_info_sym'])
                relocs.append((r['r_offset'], r['r_info_type'], sym.name, sym['st_shndx'], sym['st_value']))
    off = addr - 0x80041000 + 0x800
    tw = [struct.unpack_from('<I', img, off + i * 4)[0] for i in range(size // 4)]
    cw = [struct.unpack_from('<I', tdata, i)[0] for i in range(0, len(tdata), 4)]
    mism, unres = [], []
    his = {}
    relocs.sort()
    for o, ty, name, shndx, sval in relocs:
        wi = o // 4
        if wi >= len(tw): continue
        if shndx == 'SHN_UNDEF' or shndx == 0:
            a = amr.sym_addr(name, tab)
            if a is None:
                unres.append(name); continue
        else:
            a = None                      # local label (.text/.rodata) -- checked by the byte compare
        w = cw[wi]; r = tw[wi]
        if ty == 4 and a is not None:     # R_MIPS_26
            want = (a >> 2) & 0x3ffffff
            if (r & 0x3ffffff) != want:
                mism.append((wi, 'jal', name, hex(a), hex((r & 0x3ffffff) << 2 | 0x80000000)))
        elif ty == 6 and a is not None:   # LO16: addend in the word
            add = w & 0xffff; add = add - 0x10000 if add >= 0x8000 else add
            want = (a + add) & 0xffff
            if (r & 0xffff) != want:
                mism.append((wi, '%lo', name, hex(a + add), hex(r & 0xffff)))
        elif ty == 5 and a is not None:   # HI16: pair with the next LO16 of the same symbol for the carry
            # ⚠️ Pair on the REGISTER, not just the symbol (fixed 2026-09-20).
            # Matching "the next LO16 of the same symbol" by offset alone
            # cross-pairs whenever two `lui`s for one symbol are live in
            # different registers: in config_screen_init the `lui v0` at
            # word 47 stole the `%lo` at word 48, which belongs to the
            # `lui at` at word 45, and the audit reported a MISMATCH on a
            # function that is byte-identical to retail.  A %lo belongs to the
            # lui that wrote the base register it addresses through.
            hi_reg = (w >> 16) & 0x1f          # lui rt
            lo = next(((o2, w2) for o2, ty2, n2, s2, v2 in relocs
                       if ty2 == 6 and n2 == name and o2 > o
                       for w2 in [cw[o2 // 4]]
                       if ((w2 >> 21) & 0x1f) == hi_reg), None)
            add = 0
            if lo is not None:
                add = lo[1] & 0xffff; add = add - 0x10000 if add >= 0x8000 else add
            want = ((a + add + 0x8000) >> 16) & 0xffff
            if (r & 0xffff) != want:
                mism.append((wi, '%hi', name, hex(a + add), hex(r & 0xffff)))
    return mism, unres, len(relocs)

def main():
    tab = amr.load_symtab(); img = open(os.path.join(ROOT, 'disks/SLPS_000.65'), 'rb').read()
    names = [a for a in sys.argv[1:] if not a.startswith('--')]
    reg = json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))['functions']
    if '--all' in sys.argv:
        names = [e['name'] for e in reg if e['source'].endswith('.c')]
    if '--registered-since' in sys.argv:
        rev = sys.argv[sys.argv.index('--registered-since') + 1]
        old = subprocess.run(['git', 'show', f'{rev}:config/level3_functions.json'], cwd=ROOT,
                             capture_output=True, text=True, check=True).stdout
        before = {e['name'] for e in json.loads(old)['functions']}
        names = [e['name'] for e in reg if e['source'].endswith('.c') and e['name'] not in before]
    reg_names = {e['name'] for e in reg}
    bad = 0
    for fn in names:
        if fn not in reg_names:
            # linked library members and other non-registry names carry no C
            # source and no C-registry relocs; the gate is inapplicable, not failed
            print(f'{fn:28s} SKIP    not a C-registry function (nothing to audit)')
            continue
        try:
            mism, unres, n = audit(fn, tab, img)
        except Exception as e:
            print(f'{fn:28s} ERROR {e}'); bad += 1; continue
        # 2026-10-03 (red-team finding 4): an UNRESOLVED name is a failure --
        # nothing checks its address (see audit_main_relocs.py).
        v = 'MISMATCH' if mism else ('UNRESOLVED' if unres else 'OK')
        if mism or unres: bad += 1
        print(f'{fn:28s} {v:8s} relocs={n} unresolved={sorted(set(unres))}' + (f' -> {mism}' if mism else ''))
    sys.exit(1 if bad else 0)

if __name__ == '__main__':
    main()
