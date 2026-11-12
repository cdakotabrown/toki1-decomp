/* Address: 0x8008E61C */
/* Size: 60 bytes */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions 0x14 0x18 */
/* @PostProc: swap_instructions 0x18 0x1c */
/* @PostProc: swap_instructions 0x1c 0x20 */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80080000-0x800B0000) */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

extern void SpuVmKeyOff(s32 a0, s16 a1, s16 a2, u16 a3);

void SpuVmSeKeyOff(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SpuVmKeyOff(0x20, (s16)arg0, (s16)arg1, (u16)arg2);
}
