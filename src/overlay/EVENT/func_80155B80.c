/* @SymbolStart: 0x80155B78 */
/* @SymbolSize: 200 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: patch_insn_reg_field 0x98:rt:v1:v0 0x9c:rs:v1:v0 */
/* Address: 0x80155B80 (window starts at the absorbed cross-prologue head */
/* Size: 200 bytes (50 insns) including the head */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800B0B53;
extern s32 D_800EECBC;
extern u8 D_800B1AF6;
extern volatile s16 D_80094730;

extern void func_80011DFC(void);
extern void func_8004CF30(void);

s32 func_80155B80(void)
{
    s32 v;
    s32 saved;

    v = D_800B1746;
    if (v == 6 && D_800B0B53 != 0 && D_800EECBC == 2) {
        D_80094730 = 0;
        D_800B1AF6 = D_800B1AF6 - 5;
        func_80011DFC();
        return;
    }
    if (v == 8 && D_800EECBC == 2) {
        saved = D_800B1AF6;
        func_8004CF30();
        v = D_800B1AF6;
        if (saved != v) {
            D_80094730 = 0;
            v = (u8)(v - 5);
            D_800B1AF6 = v;
        }
        return v;
    }
    return;
}
