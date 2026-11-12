/* Debug Symbol: GsGetVcount (from o.bin, exact match) */
/* Address: 0x80099640 */
/* Size: 36 bytes */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
typedef unsigned int u32;

extern void GetRCnt(u32 arg);

void GsGetVcount(void) {
    GetRCnt(0xF2000001);
}
