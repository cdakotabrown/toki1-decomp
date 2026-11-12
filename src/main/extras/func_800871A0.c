/* Address: 0x800871A0 */
/* Size: 128 bytes */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
extern void ResetCallback(void);
extern void MDEC_reset(s32);

void func_800871A0(s32 arg0) {
    if (arg0 == 0) {
        ResetCallback();
    }
    MDEC_reset(arg0);
}
