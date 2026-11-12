#!/usr/bin/env python3
"""find_main_swaps.py - propose `@PostProc: swap_instructions` for a MAIN-BINARY
function whose compiled body is a REORDERING of retail's, not a different body.

    python3 tools/analysis/find_main_swaps.py <FUNC> <ADDR> <SIZE> <src.c> [--max-dist N] [--strict-move]

Runs the file's own PostProc chain, then looks for positions where retail and
the compiled object hold the same instructions in a different order.  A swap is
PROPOSED only when all of these hold, which is what keeps it from being a
disguised rewrite:

  * the two words have the same opcode as retail's words at the swapped
    positions (so the swap makes both sides agree on the instruction, and only
    register fields can be left over for gen_main_reg_patches);
  * neither word is a branch or a jump -- `swap_instructions` does not adjust
    branch displacements, and moving one silently retargets it;
  * they are independent: neither writes a register the other reads or writes,
    and neither is a `%hi`/`%lo` half whose partner sits between them.

Each candidate is then VERIFIED by applying it and re-running
gen_main_reg_patches: it is kept only if the not-paintable count actually
drops.  That is the check that matters -- the static rules above are necessary,
not sufficient.

Emits the directives in the order they must appear in the file (they apply
sequentially, so the offsets are positions in the object as it stands at that
point in the chain).

⚠️ --strict-move (added 2026-09-17) fixes two limitations of the default rules
above, and is what you want on a GCC/ASPSX library function whose residual is
the instruction SCHEDULE:

  * the default candidate filter keys on `op(target[i]) != op(compiled[i])`, so
    it cannot see a PURE REORDERING -- a block-move run of `lw`/`sw` emitted in
    the opposite order, which is the commonest ASPSX-vs-gcc schedule difference
    in the library region.  --strict-move considers any position whose WORD
    differs.
  * "independent of each other" is not sufficient for a NON-ADJACENT swap.
    Exchanging positions i and j moves each word past everything between them,
    so both must be independent of every word they cross.  The default rules
    will happily propose a swap that carries a definition across its own use.
    --strict-move checks every crossed word, requires memory operands to be
    provably disjoint (same base register, different immediate) unless both are
    loads, refuses to move a word into or out of a delay slot, and refuses to
    separate a `lui` from a consumer of its register.

The default behaviour is unchanged so that results already recorded in file
headers stay reproducible.
"""
import sys, os, re, subprocess, tempfile, shutil
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
import dump_main_diff as M

BRANCH_OPS = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17}
LOAD_STORE = {0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x28,0x29,0x2a,0x2b,0x2e}

def op(w):   return (w >> 26) & 0x3f
def rs(w):   return (w >> 21) & 0x1f
def rt(w):   return (w >> 16) & 0x1f
def rd(w):   return (w >> 11) & 0x1f

def is_branchy(w):
    o = op(w)
    return o in BRANCH_OPS or o in (2, 3) or (o == 0 and (w & 0x3f) in (8, 9))

def defs(w):
    o = op(w)
    if o == 0:  return {rd(w)} - {0}
    if o in (2, 3) or o in BRANCH_OPS: return set()
    if o in {0x28,0x29,0x2a,0x2b,0x2e}: return set()      # stores define nothing
    return {rt(w)} - {0}

def uses(w):
    o = op(w)
    if o == 0:  return {rs(w), rt(w)} - {0}
    if o in (2, 3): return set()
    if o in {0x28,0x29,0x2a,0x2b,0x2e} or o in BRANCH_OPS: return {rs(w), rt(w)} - {0}
    if o == 0x0f: return set()                             # lui
    return {rs(w)} - {0}

def independent(a, b):
    return not (defs(a) & (defs(b) | uses(b))) and not (defs(b) & uses(a))


LOADS  = {0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x30,0x31,0x32,0x33,0x35,0x37}
STORES = {0x28,0x29,0x2a,0x2b,0x2e,0x38,0x39,0x3a,0x3b,0x3e,0x3f}


def mem_ref(w):
    o = op(w)
    if o in LOADS:  return ('L', rs(w), w & 0xffff)
    if o in STORES: return ('S', rs(w), w & 0xffff)
    return None


def mem_disjoint(a, b):
    """True if a and b may be reordered as far as MEMORY is concerned."""
    ma, mb = mem_ref(a), mem_ref(b)
    if ma is None or mb is None:  return True       # at most one touches memory
    if ma[0] == 'L' and mb[0] == 'L': return True   # two loads never conflict
    return ma[1] == mb[1] and ma[2] != mb[2]        # same base, different offset


def crossable(a, b):
    return independent(a, b) and mem_disjoint(a, b)


def sound_move(c, i, j):
    """c[i] and c[j] may be EXCHANGED -- each moves past everything between."""
    a, b = c[i][1], c[j][1]
    if a == b: return False
    for k in (i, j):
        if is_branchy(c[k][1]): return False
        if k > 0 and is_branchy(c[k-1][1]): return False   # k is a delay slot
    if not crossable(a, b): return False
    for k in range(i + 1, j):
        w = c[k][1]
        if is_branchy(w): return False
        if not crossable(a, w) or not crossable(b, w): return False
        if op(a) == 0x0f and rt(a) in uses(w): return False      # lui / consumer
        if op(w) == 0x0f and rt(w) in (uses(a) | uses(b)): return False
    return True

def unpaintable(fn, addr, size, src):
    r = subprocess.run([sys.executable, os.path.join(ROOT,'tools/analysis/gen_main_reg_patches.py'),
                        fn, '0x%08X' % addr, str(size), src],
                       cwd=ROOT, capture_output=True, text=True)
    m = re.search(r'NOT paintable: (\d+)', r.stdout)
    return int(m.group(1)) if m else 10**6

def add_swap(src_text, a, b):
    line = "/* @PostProc: swap_instructions --offset1 0x%x --offset2 0x%x */\n" % (a, b)
    m = re.search(r'(?m)^\s*(?:/\*|\*)?\s*@PostProc: (?!patch_insn_reg_field)[^\n]*\n', src_text)
    idx = 0
    for m in re.finditer(r'(?m)^[^\n]*@PostProc: (?!patch_insn_reg_field)[^\n]*\n', src_text):
        idx = m.end()
    return src_text[:idx] + line + src_text[idx:]

def main():
    fn, addr, size, src = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3]), sys.argv[4]
    strict = '--strict-move' in sys.argv
    maxd = 14 if strict else 3
    if '--max-dist' in sys.argv: maxd = int(sys.argv[sys.argv.index('--max-dist')+1])
    M.ADDR_S = '0x%08X' % addr
    work = tempfile.NamedTemporaryFile('w', suffix='.c', delete=False).name
    shutil.copy(src, work)
    best = unpaintable(fn, addr, size, work)
    print("start: %d not paintable%s" % (best, "  (--strict-move)" if strict else ""))
    if best >= 10**6:
        print("BASELINE FAILED -- the source does not build; nothing to derive.")
        return
    kept = []
    improved = True
    while improved:
        improved = False
        o = M.build(work, fn)
        t = M.target(addr, size); c = M.words(o, fn)
        n = min(len(t), len(c))
        for i in range(n):
            if strict:
                if t[i][1] == c[i][1]: continue
            elif op(t[i][1]) == op(c[i][1]):
                continue
            for j in range(i+1, min(i+1+maxd, n)):
                if strict:
                    if t[j][1] == c[j][1]: continue
                    if not sound_move(c, i, j): continue
                else:
                    if op(t[i][1]) != op(c[j][1]) or op(t[j][1]) != op(c[i][1]): continue
                    if is_branchy(c[i][1]) or is_branchy(c[j][1]): continue
                    if not independent(c[i][1], c[j][1]): continue
                txt = open(work).read()
                trial = add_swap(txt, i*4, j*4)
                open(work,'w').write(trial)
                u = unpaintable(fn, addr, size, work)
                if u < best:
                    best = u; kept.append((i*4, j*4)); improved = True
                    warn = ""
                    if not strict and j > i + 1:
                        warn = "   <-- NON-ADJACENT, soundness UNCHECKED"
                    print("  swap 0x%x <-> 0x%x  -> %d not paintable%s"
                          % (i*4, j*4, u, warn))
                    break
                open(work,'w').write(txt)
            if improved: break
    print("\n%d swap(s), %d not paintable left" % (len(kept), best))
    if not strict:
        far = [(a, b) for a, b in kept if b - a > 4]
        if far:
            print("\n!! %d of these swaps are NON-ADJACENT and their soundness was NOT"
                  % len(far))
            print("!! checked.  The default rules only test that the two words are")
            print("!! independent OF EACH OTHER, but exchanging positions i and j moves")
            print("!! each word past everything between them, so both must be")
            print("!! independent of every word they cross.  A swap that carries a")
            print("!! definition across its own use will be proposed happily, and")
            print("!! gen_main_reg_patches cannot catch it -- it only measures byte")
            print("!! closeness.  Re-run with --strict-move before shipping these:")
            for a, b in far:
                print("!!   0x%x <-> 0x%x" % (a, b))
    for a, b in kept:
        print(" * @PostProc: swap_instructions --offset1 0x%x --offset2 0x%x" % (a, b))

if __name__ == '__main__':
    main()
