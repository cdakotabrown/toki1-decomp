/* Function: GsMapModelingData   (the map name; it is almost certainly */
/* Address: 0x8009D230 recorded; the real function starts at 0x8009D220 */
/* Size: 112 bytes (28 instructions) over the absorbed window */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: Psy-Q GCC 2.6.0 */
/* @SymbolStart: 0x8009D220 */
/* @SymbolSize: 112 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-rerun-cse-after-loop -fno-schedule-insns2 --fill-epilogue */
/* @PostProc: adjust_branch_offset --offset 0xc --imm 0x16 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: adjust_branch_offset --offset 0x28 --imm 0xf */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x48 */
void GsMapModelingData(long *p)
{
    long n, i;

    if (p[0] & 1) {
        return;
    }
    p[0] |= 1;
    n = p[1];
    p++;
    for (i = 0; i < n; i++) {
        long *e = p + 1;

        e[i * 7 + 0] += (long)e;
        e[i * 7 + 2] += (long)e;
        e[i * 7 + 4] += (long)e;
    }
}
