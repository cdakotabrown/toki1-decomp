/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 30 --to 43 */
/* @PostProc: remap_stack_slots --from 31 --to 41 */
/* @PostProc: patch_insn_reg_field 0x38:rd:t7:t8 0x3c:rs:t7:t8 0x44:rt:t8:t9 0x48:rs:t8:t9 0x48:rt:t8:t9 0x4c:rt:t9:t0 0x50:rs:t8:t9 0x58:rs:t9:t0 0x80:rt:t0:t1 0x84:rs:t0:t1 0x84:rt:t0:t1 0x8c:rs:t0:t1 0x94:rt:t1:t2 0x9c:rs:t1:t2 0xb8:rt:t2:t3 0xbc:rt:t3:t4 0xc0:rs:t3:t4 0xc0:rt:t3:t4 0xc4:rs:t2:t3 0xc4:rt:t2:t3 0xcc:rs:t2:t3 0xcc:rt:t3:t4 0xf0:rt:t4:t5 0xf4:rs:t4:t5 0xf4:rt:t4:t5 0xfc:rd:t5:t7 0xfc:rt:t4:t5 0x100:rs:t5:t7 0x104:rt:t6:t8 0x108:rs:t6:t8 0x108:rt:t6:t8 0x10c:rt:t7:t9 0x110:rs:t6:t8 0x118:rs:t7:t9 0x134:rt:t8:t0 0x138:rt:t9:t1 0x13c:rs:t9:t1 0x13c:rt:t9:t1 0x140:rs:t8:t0 0x140:rt:t8:t0 0x148:rs:t8:t0 0x148:rt:t9:t1 0x158:rt:t0:t2 0x15c:rt:t1:t3 0x160:rs:t1:t3 0x160:rt:t1:t3 0x164:rs:t0:t2 0x164:rt:t0:t2 0x16c:rs:t0:t2 0x16c:rt:t1:t3 */
/* func_8013EAFC -- GYOZI overlay, Address: 0x8013EAFC, Size: 392 bytes */
#include "types.h"

extern s32 system_config_disp(s32 arg0);
extern s32 func_8005E1C0(s32 arg0);
extern void func_8004DE1C(void);

extern s16 D_800F5556;
extern s16 D_800F56DE;
extern s32 D_800F56E0;
extern s16 D_800F574E;
extern s32 D_800F5750;
extern u8 D_800F5902;
extern u8 D_800F592F;
extern u8 D_800F593D;
extern u8 D_801486E4;

void func_8013EAFC(void)
{
    u8 sel;
    u8 st;
    s32 ret;

    st = system_config_disp(7);
    ret = func_8005E1C0(7);
    sel = 255;
    if (((D_800F56E0 << 30) < 0) && (D_800F592F == 0) && (st == 128) &&
        (ret == 0)) {
        sel = 7;
    }

    st = system_config_disp(0);
    ret = func_8005E1C0(0);
    if ((D_800F5902 == 0) && (st == 128) && (ret == 0)) {
        if ((sel == 255) ||
            ((sel == 7) && (D_800F5556 < D_800F56DE))) {
            sel = 0;
        }
    }

    st = system_config_disp(9);
    ret = func_8005E1C0(9);
    if (((D_800F5750 << 30) < 0) && (D_800F593D == 0) && (st == 128) &&
        (ret == 0)) {
        if ((sel == 255) ||
            ((sel == 7) && (D_800F574E < D_800F56DE)) ||
            (!sel && (D_800F574E < D_800F5556))) {
            sel = 9;
        }
    }

    D_801486E4 = sel;
    func_8004DE1C();
}
