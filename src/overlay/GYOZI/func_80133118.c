/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 */
/* func_80133118 -- GYOZI overlay, Address: 0x80133118, Size: 408 bytes */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0xfc */
/* @PostProc: patch_insn_reg_field 0x24:rt:v0:v1 0x28:rs:v0:v1 0x28:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x40:rt:a0:v0 0x44:rs:a0:v0 0x44:rt:a0:v0 0x48:rt:a1:v1 0x4c:rs:a0:v0 0x54:rs:a0:v0 0x54:rt:v1:v0 0x58:rd:v0:t6 0x58:rt:v1:v0 0x5c:rt:v0:t6 0x7c:rt:a0:v0 0x80:rs:a0:v0 0x80:rt:a0:v0 0x88:rs:a0:v0 0xa0:rs:a1:v1 0xa0:rt:a1:v1 0xa8:rs:a1:v1 0xb0:rt:a1:v1 0xe8:rt:a1:v1 0xec:rt:a1:v1 0xf0:rs:a1:v1 0xf0:rt:a1:v1 0xf8:rs:a1:v1 0xf8:rt:t7:v0 0xfc:rs:a1:v1 0x100:rs:t7:v0 0x12c:rt:v0:v1 0x130:rs:v0:v1 0x130:rt:v0:v1 0x138:rs:v0:v1 */
#include "types.h"

extern s32 func_8005B32C(void);
extern void func_80086AB0(s32 a0);
extern void func_8005B06C(s32 a0);
extern void func_801343A4(void);

extern u8 D_800C3370[];
extern u8 D_800F62CF;
extern s32 D_800F6474;
extern u8 D_800F647D;
extern s16 D_80145EB8;
extern s16 D_80145F64;

s32 func_80133118(void)
{
    s32 state;
    s32 counter;
    s32 flags;
    s32 idx;
    s32 value;
    s32 cond;
    u8 tmp8;
    s32 tmp;

    if (func_8005B32C() == 0) {
        goto late_phase;
    }
    state = D_80145EB8;
    if (state >= 5) {
        goto late_phase;
    }

    if (state == 0) {
        counter = D_80145F64;
        if (counter < 90) {
            counter = counter + 1;
            value = (s16)counter;
            D_80145F64 = value;
            if (value == 1) {
                func_80086AB0(1546);
            }
            if (D_80145F64 == 30) {
                func_80086AB0(1540);
            }
            return 0;
        }

        flags = D_800F647D;
        if ((flags & 7) == 0) {
            idx = D_800F62CF * 7 + (s32)((u32)flags >> 3);
            value = D_800C3370[idx] + 1285;
            value = (u16)((value & 0xFF) | 0x600);
            D_800F647D = flags;
            func_80086AB0(value);
        }
    }

    tmp8 = D_800F647D;
    idx = 1;
    cond = ((u32)tmp8 < 64);
    tmp8 = tmp8 + 1;
    if (cond != 0) {
        D_800F647D = tmp8;
        return 0;
    }
    D_800F647D = 0;
    func_8005B06C(idx);

late_phase:
    func_801343A4();
    state = D_80145EB8;
    if (state < 5) {
        tmp = D_800F6474;
        cond = (tmp == 1);
        D_800F6474 = tmp + 1;
        if (cond != 0) {
            func_80086AB0(1544);
        }
    }
    return;
}
