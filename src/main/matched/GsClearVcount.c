/* Address: 0x8009CA80 */
/* Size: 36 bytes */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
typedef unsigned int u32;

extern void ResetRCnt(u32 arg);

void GsClearVcount(void) {
    ResetRCnt(0xF2000001);
}
