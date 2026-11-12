/* Address: 0x8008B530 */
/* Size: 32 bytes (8 instructions) */
/* Compiler: IDO 5.2 with -O2 -G0 -mips1 -EL -non_shared */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* Post-processing required:
 *   Convert OR to ADDU for move instruction (IDO region uses ADDU encoding)
 */
extern void _SsInit(int);

void SsInit(void) {
    _SsInit(0);
}
