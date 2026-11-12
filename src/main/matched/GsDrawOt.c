/* Debug Symbol: GsDrawOt (from o.bin, high match) */
/* Address: 0x8009CB60 */
/* Size: 36 bytes (9 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
extern void DrawOTag(int arg0);

void GsDrawOt(int *arg0) {
    DrawOTag(arg0[4]);
}
