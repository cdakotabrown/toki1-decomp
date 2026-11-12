/* func_80133280 -- GYOZI overlay, Address: 0x80133280, Size: 416 bytes */
/* @PostProc: swap_instructions --offset1 0xE0 --offset2 0xE8 */
/* @PostProc: swap_instructions --offset1 0xF8 --offset2 0xFC */
/* @PostProc: patch_insn_reg_field 0x24:rt:v0:v1 0x28:rs:v0:v1 0x28:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x48:rt:t9:v1 0x5c:rd:v0:t7 0x64:rt:v0:t7 0x6c:rs:v0:t7 0x7c:rt:t8:v0 0x80:rs:t8:v0 0x80:rt:t8:v0 0x88:rs:t8:v0 0xa0:rs:t9:v1 0xa0:rt:v0:v1 0xa4:rt:t1:t9 0xa8:rs:v0:v1 0xa8:rt:t0:t8 0xac:rs:t0:t8 0xb0:rd:t3:t1 0xb0:rt:v0:v1 0xb4:rs:t1:t9 0xb4:rt:t1:t9 0xb8:rt:v1:a0 0xbc:rd:t2:t0 0xbc:rt:t1:t9 0xc0:rd:t2:t0 0xc0:rs:t2:t0 0xc0:rt:t1:t9 0xc4:rd:t4:t2 0xc4:rs:t2:t0 0xc4:rt:t3:t1 0xc8:rd:v1:a0 0xc8:rs:v1:a0 0xc8:rt:t4:t2 0xcc:rs:v1:a0 0xcc:rt:v1:a0 0xd4:rs:v1:a0 0xd4:rt:v1:a0 0xd8:rs:v1:a0 0xd8:rt:a0:t3 0xdc:rs:a0:t3 0xdc:rt:t5:t4 0xe0:rs:t5:t4 0xe8:rt:v0:v1 0xec:rt:v0:v1 0xf0:rs:v0:v1 0xf0:rt:v0:v1 0xf8:rs:v0:v1 0xf8:rt:at:v0 0xfc:rs:v0:v1 0x100:rs:at:v0 0x104:rt:t7:t6 0x10c:rt:t7:t6 0x12c:rt:v0:v1 0x130:rs:v0:v1 0x130:rt:v0:v1 0x134:rt:t8:t7 0x138:rs:v0:v1 0x144:rs:t8:t7 0x144:rt:t9:t7 0x14c:rs:t9:t7 0x15c:rt:v0:v1 0x160:rs:v0:v1 0x160:rt:v0:v1 0x16c:rs:v0:v1 0x170:rt:t0:t8 0x174:rs:t0:t8 0x174:rt:t1:t8 0x17c:rs:t1:t8 0x18c:rt:t2:t9 0x190:rs:t2:t9 0x190:rt:t2:t9 0x198:rs:t2:t9 0x198:rt:t3:t0 0x19c:rt:t3:t0 */
#include "types.h"

extern s32 func_8005B32C(void);
extern void func_80086AB0(s32 a0);
extern void func_8005B06C(s32 a0);
extern void func_801343A4(void);

extern u8 D_800C3370[];
extern u8 D_800F62CF;
extern s32 D_800F6474;
extern u8 D_800F647D;
extern volatile s16 D_80145EB8;
extern volatile s16 D_80145F64;

s32 func_80133280(void)
{
    s32 state;
    s32 counter;
    s32 flags;
    s32 idx;
    s32 value;
    u8 *pg;
    u8 *qg;
    u8 work;

    if (func_8005B32C() == 0) {
        goto late_phase;
    }
    state = D_80145EB8;
    if (state >= 3) {
        goto late_phase;
    }

    if (state == 0) {
        counter = D_80145F64;
        pg = (u8 *)0x800F0000;
        if (counter < 90) {
            counter = (s16)(counter + 1);
            D_80145F64 = counter;
            if (counter == 1) {
                func_80086AB0(1546);
            }
            if (D_80145F64 == 30) {
                func_80086AB0(1540);
            }
            return 0;
        }

        flags = pg[0x647D];
        if ((flags & 7) == 0) {
            idx = D_800F62CF * 7 + (s32)((u32)flags >> 3);
            value = D_800C3370[idx] + 1285;
            work = flags;
            D_800F647D = work;
            func_80086AB0((u16)((value & 0xFF) | 0x600));
        }
    }

    value = D_800F647D;
    work = value + 1;
    if ((u32)value < 64) {
        D_800F647D = work;
        return 0;
    }
    D_800F647D = 0;
    func_8005B06C(1);

late_phase:
    func_801343A4();
    state = D_80145EB8;
    qg = (u8 *)0x800F0000;
    if (state < 3) {
        if (*(s32 *)(qg + 0x6474) == 1) {
            func_80086AB0(1544);
            state = D_80145EB8;
        }
    }
    if (state == 3) {
        if (*(s32 *)(qg + 0x6474) == 1) {
            func_80086AB0(1547);
        }
    }
    D_800F6474 = D_800F6474 + 1;
}
