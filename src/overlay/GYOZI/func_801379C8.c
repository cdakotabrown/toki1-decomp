/* @SymbolStart: 0x801379C0 */
/* @SymbolSize: 216 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t7:t8 0x30:rs:t7:t8 0x30:rt:t8:t9 0x34:rt:t8:t9 0x40:rt:t3:t4 0x4c:rt:t4:t5 0x64:rt:t9:t0 0x70:rt:t9:t0 0x7c:rt:t0:t1 0x88:rt:t0:t1 0x90:rt:t2:t3 0x94:rt:t1:t2 0xa0:rt:t1:t2 0xa8:rt:t2:t3 0xac:rs:t3:t4 0xac:rt:t3:t4 0xb4:rs:t4:t5 0xb4:rt:t4:t5 0xb8:rt:t3:t4 0xc0:rt:t5:t6 0xc4:rs:t5:t6 0xc4:rt:t5:t6 0xc8:rt:t4:t5 0xd4:rt:t5:t6 */
/* func_801379C8 -- GYOZI overlay, Address: 0x801379C0, Size: 216 bytes (54 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80147240;
extern u8 D_800F5750;
extern s32 D_801470F4;
extern s32 D_80147128;
extern s32 D_8014715C;
extern s32 D_8014718C;
extern s32 D_80147190;
extern s32 D_80147194;
extern s16 D_80147198;
extern s16 D_8014723C;

void func_801379C8(void)
{
    u32 temp = D_80147240;
    s32 v0;

    if (temp != 0) {
        D_800F5750 |= 2;
    }
    v0 = D_8014723C;
    if (v0 >= 7) {
        D_80147198 = 0;
    } else if (v0 >= 5) {
        D_80147198 = 1;
    } else if (v0 >= 3) {
        D_80147198 = 2;
    } else if (v0 > 0) {
        D_80147198 = 3;
    } else {
        D_80147198 = 4;
    }
    D_8014718C = D_801470F4;
    D_80147190 = D_80147128;
    D_80147194 = D_8014715C;
    func_8004DE1C();
}
