#!/usr/bin/env python3
"""Extract a dispatcher's jr (switch) table for an overlay function.

Two strategies, auto-selected by the dispatcher's table base:
  - IN-EXN  (base >= 0x80132000): the table is static rodata in the .EXN; read it
            directly at file_off = (base+offset) - 0x80132000.
  - RUNTIME (base  < 0x80132000, e.g. 0x80120000): the table is built at runtime
            by the overlay's init code via lui/ori(/addiu)/sw store-sequences.
            We reconstruct that RAM region with a forward abstract-interpretation
            pass over the whole .EXN text (per-register value tracking), then read
            the table out of the reconstructed image.

The dispatcher shape (IDO -O2, MIPS I):
    sltiu at, $idx, K
    beqz  at, <default>
    sll   $idx, $idx, 2
    lui   at, HI
    addu  at, at, $idx
    lw    $t, OFFSET(at)
    jr    $t

Usage:
    python3 tools/overlay/extract_jr_table.py EVENT 0x8015300C
    python3 tools/overlay/extract_jr_table.py NAME_ENT func_XXXXXXXX
"""
import argparse, os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TEXT_BASE = 0x80132000
RAM_BASE = 0x80000000


def op(w):       return w >> 26
def rs(w):       return (w >> 21) & 0x1F
def rt(w):       return (w >> 16) & 0x1F
def rd(w):       return (w >> 11) & 0x1F
def funct(w):    return w & 0x3F
def imm(w):      return w & 0xFFFF
def simm(w):     return (w & 0xFFFF) - 0x10000 if (w & 0x8000) else (w & 0xFFFF)


def load_exn(overlay):
    path = os.path.join(ROOT, "assets", "overlays", f"{overlay}.EXN")
    data = open(path, "rb").read()
    n = len(data) // 4
    return [struct.unpack("<I", data[i * 4:i * 4 + 4])[0] for i in range(n)], data


def find_dispatcher(words, fstart_idx):
    """Locate the jr-table dispatch within a function. Return dict or None.

    Scans from the function start to its own `jr $ra` (or a 2048-insn cap) —
    the old fixed 40-insn window silently missed any dispatcher sitting deeper
    (RPG_BAT/func_801529B4's jr is at ~insn 123; several attempts had to dump
    tables from the EXN by hand because of this)."""
    seen_jr_ra = 0
    for i in range(fstart_idx, min(fstart_idx + 2048, len(words) - 1)):
        w = words[i]
        if seen_jr_ra:
            break  # past the function's jr-ra delay slot
        if op(w) == 0 and funct(w) == 0x08 and rs(w) == 31:
            seen_jr_ra = 1  # jr $ra: allow the delay slot then stop
            continue
        # jr $t  (not jr $ra)
        if op(w) == 0 and funct(w) == 0x08 and rs(w) != 31:
            jreg = rs(w)
            # walk back for: lw $t, OFFSET(base); lui base,HI; addu base,base,idx; sltiu at,idx,K
            base_reg = off = hi = idx_reg = bound = None
            for j in range(i - 1, max(i - 12, fstart_idx) - 1, -1):
                v = words[j]
                if off is None and op(v) == 0x23 and rt(v) == jreg:      # lw $t, off(base)
                    base_reg, off = rs(v), simm(v)
                elif base_reg is not None and op(v) == 0 and funct(v) == 0x21 and rd(v) == base_reg:  # addu base,base,idx
                    idx_reg = rt(v) if rs(v) == base_reg else rs(v)
                elif base_reg is not None and op(v) == 0x0F and rt(v) == base_reg:  # lui base, HI
                    hi = imm(v)
                elif idx_reg is not None and op(v) == 0x0B:               # sltiu at, idx, K
                    bound = imm(v)
            if off is not None and hi is not None:
                return {"jr_idx": i, "table_base": (hi << 16) + off, "bound": bound}
    return None


def read_inline_table(words, table_va, bound):
    fi = (table_va - TEXT_BASE) // 4
    return [words[fi + k] for k in range(bound)]


def reconstruct_runtime_region(words):
    """Forward abstract interpretation: track GPR materialized values, commit on
    `sw R, off(at)` where at holds an absolute base. Returns {addr: value}."""
    region = {}
    st = [None] * 32  # None | ('HI', val) | ('PTR', val)
    for w in words:
        o = op(w)
        if o == 0x0F:                              # lui rt, imm
            st[rt(w)] = ("HI", imm(w) << 16)
        elif o == 0x0D:                            # ori rt, rs, imm
            s = st[rs(w)]
            st[rt(w)] = ("PTR", s[1] | imm(w)) if s and s[0] == "HI" else None
        elif o == 0x09:                            # addiu rt, rs, imm (sign-extended)
            s = st[rs(w)]
            st[rt(w)] = ("PTR", (s[1] + simm(w)) & 0xFFFFFFFF) if s and s[0] == "HI" else None
        elif o == 0x2B:                            # sw rt, off(rs)
            base = st[rs(w)]
            val = st[rt(w)]
            if base and base[0] == "HI" and val and val[0] == "PTR":
                region[(base[1] + simm(w)) & 0xFFFFFFFF] = val[1]
            # storing rt does not change any register state
        else:
            # any other op clobbers its destination(s); be conservative
            if o == 0:                             # special: rd is dest (most)
                if funct(w) not in (0x08,):        # jr has no dest
                    st[rd(w)] = None
            elif o in (0x20, 0x21, 0x23, 0x24, 0x25, 0x0C, 0x0A, 0x0B, 0x0E):  # loads/imm-alu -> rt dest
                st[rt(w)] = None
    return region


def read_ram_table(ram_path, table_va, bound):
    """Read a RUNTIME table directly from a PS1 RAM dump (0x80000000-mapped).
    The dump MUST have been captured with the target overlay loaded *and* its
    init/relocation pass run, or the slots will be zero."""
    data = open(ram_path, "rb").read()
    off = (table_va & 0x1FFFFFFF)            # kuseg/kseg0 -> physical RAM offset
    return [struct.unpack("<I", data[off + k * 4: off + k * 4 + 4])[0]
            for k in range(bound)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("overlay")
    ap.add_argument("func", help="address (0x...), func_XXXXXXXX or a landed overlay function name")
    ap.add_argument("--quiet", action="store_true")
    ap.add_argument("--ram", metavar="DUMP",
                    help="read RUNTIME tables from a PS1 RAM dump captured with "
                         "this overlay loaded (e.g. ram.bin). Required for the "
                         "~67 loader-relocated dispatchers; the table is NOT in "
                         "the EXN.")
    args = ap.parse_args()

    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import overlay_names
    landed = overlay_names.func_addr(args.overlay.upper(), args.func)   # a renamed function
    if landed is not None and not args.func.lower().startswith("0x"):
        addr = landed
    else:
        addr = int(args.func.replace("func_", ""), 16) if args.func.lower().startswith(("0x", "func_")) \
            else int(args.func, 16)
    words, _ = load_exn(args.overlay)
    fstart = (addr - TEXT_BASE) // 4

    disp = find_dispatcher(words, fstart)
    if not disp:
        print(f"no jr-table dispatcher found near {addr:#x}", file=sys.stderr)
        sys.exit(2)
    base = disp["table_base"]
    bound = disp["bound"] or 0
    if not args.quiet:
        print(f"# {args.overlay}/func_{addr:08x}: table @ {base:#010x}, bound={bound}, "
              f"strategy={'IN-EXN' if base >= TEXT_BASE else 'RUNTIME'}")
    if base >= TEXT_BASE:
        entries = read_inline_table(words, base, bound)
    elif args.ram:
        entries = read_ram_table(args.ram, base, bound)
    else:
        region = reconstruct_runtime_region(words)
        entries = [region.get((base + k * 4) & 0xFFFFFFFF, 0) for k in range(bound)]

    if entries and all(e == 0 for e in entries):
        hint = ("Pass --ram <dump_with_THIS_overlay_loaded> (the repo ram.bin was "
                "captured with NAME_ENT, which has NO runtime tables, so it cannot "
                "resolve EVENT/ETC). The table is written by the overlay loader's "
                "relocation pass into runtime-allocated RAM — it is NOT in the EXN "
                "(verified: case-body addresses appear nowhere in the .EXN as "
                "absolute, text-relative, or 0x80120000-relative words), so there "
                "is no analytic recovery; a per-overlay RAM dump is required.")
        if args.ram:
            hint = ("--ram dump has this table region zeroed — the overlay was not "
                    "loaded (or its init had not run) when the dump was captured.")
        print(f"# WARNING: all {bound} entries zero. {hint}", file=sys.stderr)
    # Sanity: a correctly-resolved EXN table points into the loaded image
    # (0x80132000-0x80162000). Flag entries that do not.
    OVL_LO, OVL_HI = TEXT_BASE, TEXT_BASE + 0x30000
    for k, e in enumerate(entries):
        rel = f"  (func+0x{e - addr:x})" if addr <= e < addr + 0x4000 else ""
        flag = "" if (e == 0 or OVL_LO <= e < OVL_HI) else "  <-- NOT in overlay image"
        print(f"  case {k:2}: 0x{e:08x}{rel}{flag}")


if __name__ == "__main__":
    main()
