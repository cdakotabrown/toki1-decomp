/* Function: DecDCTvlc */
/* Address: 0x800869E0 */
/* Size: 64 bytes (16 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* @Compiler: gcc281 */
/* @CompileFlags: -O2 -G0 -mips1 */
/* @PostProc: swap_instructions --offset1 8 --offset2 16 */
/* @PostProc: swap_instructions --offset1 12 --offset2 16 */
extern int MDEC_vlc(int, int);
extern void MDEC_vlc2(int, int);

void DecDCTvlc(int a0, int a1) {
    if (MDEC_vlc(a0, a1) == 0) {
        MDEC_vlc2(a0, a1);
    }
}
