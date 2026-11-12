/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t6 0x2c:rs:v1:t6 0x2c:rt:v1:t6 0x30:rt:a0:v1 0x34:rd:t6:t7 0x34:rt:v1:t6 0x38:rd:t6:t7 0x38:rs:t6:t7 0x38:rt:v1:t6 0x3c:rd:t6:t7 0x3c:rt:t6:t7 0x40:rd:a0:v1 0x40:rs:a0:v1 0x40:rt:t6:t7 0x44:rs:a0:v1 0x44:rt:a0:v1 0x48:rt:t9:t8 0x4c:rs:t9:t8 0x4c:rt:t9:t8 0x50:rd:t7:t9 0x50:rt:a0:v1 0x54:rd:t8:t0 0x54:rt:t7:t9 0x58:rt:t9:t0 0x64:rs:a0:v1 0x64:rt:t0:t2 0x68:rs:t0:t1 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rs:t2:t3 0x80:rs:v1:t6 0x84:rt:t3:t4 0x88:rs:t3:t4 0x88:rt:t3:t4 0x90:rs:t3:t4 0x90:rt:t4:t5 0x94:rt:t4:t5 0x98:rt:t5:t7 0x9c:rs:t5:t7 0x9c:rt:t5:t7 0xa4:rs:t5:t7 0xa4:rt:t6:t9 0xac:rt:t6:t9 0xb8:rt:t9:t8 0xbc:rs:t9:t8 0xbc:rt:t9:t8 0xc0:rd:t7:t0 0xc4:rd:t8:t1 0xc4:rt:t7:t0 0xc8:rt:t9:t1 0xcc:rt:t1:t2 0xd0:rs:t1:t2 0xd0:rt:t1:t2 0xd4:rt:t0:t3 0xd8:rs:t0:t2 0xd8:rt:t1:t3 0xdc:rt:t2:t6 0xe0:rs:t2:t6 0xe0:rt:t2:t6 0xe4:rt:t3:t5 0xe8:rs:t2:t6 0xec:rs:t3:t5 0xf4:rt:t8:t1 0xf8:rt:t4:t7 0xfc:rs:t4:t7 0xfc:rt:t4:t7 0x104:rs:t4:t7 0x104:rt:t5:t9 0x108:rt:t5:t9 0x10c:rt:t6:t0 0x110:rs:t6:t0 0x110:rt:t6:t0 0x118:rs:t6:t0 0x118:rt:t7:t8 0x120:rt:t7:t8 0x124:rs:t8:t1 0x124:rt:t8:t1 0x12c:rs:t8:t1 0x12c:rt:t9:t2 0x130:rt:t9:t2 */
/* func_801358D0 -- GYOZI overlay, Address: 0x801358D0, Size: 316 bytes (79 insns) */
#include "types.h"

extern s32 system_config_disp(s32);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern volatile u8 D_800F53DF;
extern volatile u8 D_800F53E0;
extern volatile u8 D_800F5AAE;
extern volatile u8 D_800F647A;
extern volatile u8 D_80145F60;
extern s32 D_800F5490;
extern u8 D_800F555C[];

void func_801358D0(void)
{
    s32 ret;
    s32 idx;
    u32 v;

    ret = system_config_disp(D_800F62CF);
    idx = D_800F62CF;
    v = *(u32 *)(D_800F555C + idx * 56);
    if (D_800F53E0 == ((v << 23) >> 27) && D_800F53DF == (v & 0xF) &&
        D_80145F60 == 0 && idx == 0) {
        D_800F5AAE |= 1;
        D_800F647A += 4;
    } else {
        v = (u32)D_800F5490;
        if (D_800F53E0 == ((v << 23) >> 27) && D_800F53DF == (v & 0xF) &&
            D_80145F60 == 0) {
            if ((u32)(ret & 0x7F) < 2) {
                D_800F5AAE |= 4;
                D_800F647A += 27;
            } else {
                D_800F5AAE |= 8;
            }
        }
    }
    func_8004DE1C();
}
