#!/usr/bin/env python3
"""
audit_hilo_pairing.py - check HI16/LO16 relocation pairing on the final object.

WHY THIS EXISTS (2026-09-17).  A whole-body `scheduler_normalizer` can permute
same-opcode RELOCATED words and leave their relocations behind.  The object then
carries the `%hi` of one symbol where the target has another, and it still reads
"exact" because the harness copies the target's immediate at every relocation
site.  `bg_change` reported 94.7% with 28 of 178 sites wrong that way; it would
have broken the build had it ever been registered.

`audit_reloc_positions` cannot settle this on an open function: on a body that
has not converged, relocations legitimately land where the target holds
different instructions, so it reports MISMATCH either way.  Counting mismatched
sites with and without the directive helps but is too weak when the body misses
most sites anyway (measured indistinguishable on `holiday_icon_check`, 120 vs 119).

THIS test is convergence-independent.  On MIPS a HI16 carries into the NEXT LO16
of the same symbol.  A LO16 with no preceding unconsumed HI16 has nothing to
carry into and CANNOT LINK, whatever the rest of the body looks like.  All four
files caught this way were pairing-clean without the directive and broken with
it, including the one the site count could not separate from drift.

Usage:
    python3 tools/analysis/audit_hilo_pairing.py FUNC [FUNC ...]
exit 0 = every object pairs cleanly, 1 = a defect was found.
"""
import os, sys, tempfile

sys.path.insert(0, os.path.join(os.path.dirname(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'build'))
from elftools.elf.elffile import ELFFile                      # noqa: E402
import audit_reloc_positions as arp                           # noqa: E402

HI16, LO16 = 5, 6


def pairing_defects(fn):
    """Return (defects, n_hi, n_lo); defects is a list of (word, symbol, why)."""
    import build_level3_c as b, test_single_match as t
    src, addr, size = arp.find_source(fn)
    pp = t.parse_postproc_header(src)
    fl = t.parse_compile_flags(src)
    comp = t.parse_compiler(src)
    saved = dict(os.environ)
    with tempfile.TemporaryDirectory() as td:
        objp = os.path.join(td, 'final.o')
        os.environ.update(t.parse_compile_env(src) or {})
        os.environ['L3_KEEP_OBJ'] = objp
        try:
            b.compile_c_function(src, fn, addr, postproc_steps=pp,
                                 extra_flags=fl, compiler=comp)
        finally:
            os.environ.clear(); os.environ.update(saved)
        elf = ELFFile(open(objp, 'rb'))
        symtab = elf.get_section_by_name('.symtab')
        rel = (elf.get_section_by_name('.rel.text')
               or elf.get_section_by_name('.rela.text'))
        rs = []
        if rel is not None:
            for r in rel.iter_relocations():
                sym = symtab.get_symbol(r['r_info_sym'])
                rs.append((r['r_offset'], r['r_info_type'], sym.name))
    rs.sort()
    seen, defects, nhi, nlo = set(), [], 0, 0
    for off, ty, name in rs:
        if ty == HI16:
            nhi += 1
            seen.add(name)
        elif ty == LO16:
            nlo += 1
            if name not in seen:
                defects.append((off // 4, name, 'LO16 before any HI16 for this symbol'))
    return defects, nhi, nlo


def main():
    names = [a for a in sys.argv[1:] if not a.startswith('--')]
    if not names:
        print(__doc__); return 2
    bad = 0
    for fn in names:
        try:
            defects, nhi, nlo = pairing_defects(fn)
        except Exception as e:                                # noqa: BLE001
            print('%-24s BUILD-FAIL %s' % (fn, str(e)[-90:])); bad = 1; continue
        if defects:
            bad = 1
            print('%-24s %d DEFECT(S)  (hi=%d lo=%d)' % (fn, len(defects), nhi, nlo))
            for w, name, why in defects:
                where = 'word %d' % w if w >= 0 else 'end'
                print('    %-10s %-22s %s' % (where, name, why))
        else:
            print('%-24s OK  (hi=%d lo=%d)' % (fn, nhi, nlo))
    return bad


if __name__ == '__main__':
    sys.exit(main())
