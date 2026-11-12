/* func_80135D70 -- GYOZI overlay, Address: 0x80135D70, Size: 180 bytes (45 insns) */
/* @SymbolSize: 180 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x30:rt:t6:t7 0x34:rs:t6:t7 0x34:rt:t6:t7 0x38:rt:t7:t6 0x44:rt:t8:t0 0x48:rs:t8:t0 0x48:rt:t8:t0 0x4c:rd:t0:t9 0x4c:rt:t9:t8 0x50:rs:t8:t9 0x54:rt:t1:t2 0x58:rs:t1:t2 0x60:rt:t6:t7 0x64:rt:t2:t3 0x68:rs:t2:t3 0x68:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t3:t4 0x74:rt:t3:t4 0x78:rt:t4:t5 0x7c:rs:t4:t5 0x7c:rt:t4:t5 0x84:rs:t4:t5 0x84:rt:t5:t6 0x8c:rt:t5:t6 0x90:rs:t6:t7 0x90:rt:t6:t7 0x98:rs:t6:t7 0x98:rt:t7:t8 0xa0:rt:t7:t8 0x40:rd:t9:t8 */
#include "types.h"

extern s32 system_config_disp(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern s32 D_800F5490;
extern u8 D_800F53DF;
extern u8 D_800F53E0;
extern u8 D_800F5AAE;
extern u8 D_800F647A;

void func_80135D70(void)
{
    s32 code;
    s32 v;

    code = system_config_disp(D_800F62CF) & 0x7F;
    v = D_800F5490;

    if ((v & 0xF) == D_800F53DF && ((u32)(v << 23) >> 27) == D_800F53E0) {
        if ((u32)code < 2) {
            D_800F5AAE |= 4;
            D_800F647A = D_800F647A + 5;
        } else {
            D_800F5AAE |= 8;
            func_8004DE1C();
        }
    } else {
        func_8004DE1C();
    }
}
