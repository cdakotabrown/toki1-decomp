/* Address: 0x8013BA4C */
/* Size: 392 bytes (98 instructions) */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: patch_insn_reg_field 0x54:rt:a1:t1 0x58:rt:v0:a0 0x60:rd:v1:t2 0x60:rt:v0:a0 0x64:rd:a0:t3 0x64:rt:v0:a0 0x68:rt:a1:t1 0x6c:rd:t6:t9 0x6c:rt:a1:t1 0x70:rt:t8:t4 0x74:rt:t6:t9 0x7c:rt:a1:t1 0x80:rt:a0:t3 0x84:rt:v1:t2 0x88:rt:v0:a0 0x8c:rt:a0:t3 0x90:rt:v1:t2 0x94:rt:v0:a0 0x98:rs:t8:t4 0x98:rt:t8:t4 0x9c:rt:a3:a1 0xa0:rs:a3:a1 0xa0:rt:a3:a1 0xa4:rd:t9:t5 0xa4:rt:t8:t4 0xa8:rd:t0:t6 0xa8:rs:a3:a1 0xa8:rt:t9:t5 0xac:rs:t0:t6 0xac:rt:t1:t7 0xb0:rt:t2:t8 0xb4:rt:t4:t1 0xb8:rd:t3:t9 0xb8:rs:t1:t7 0xb8:rt:t2:t8 0xbc:rt:t3:t9 0xc0:rs:t4:t1 0xc0:rt:t4:t1 0xc4:rt:t8:t5 0xc8:rd:t5:t2 0xc8:rt:t4:t1 0xcc:rd:t6:t3 0xcc:rs:a3:a1 0xcc:rt:t5:t2 0xd0:rs:t6:t3 0xd0:rt:t7:t4 0xd4:rt:t0:t7 0xd8:rd:t9:t6 0xd8:rs:t7:t4 0xd8:rt:t8:t5 0xdc:rt:t9:t6 0xe0:rs:t0:t7 0xe0:rt:t0:t7 0xe4:rt:t4:t2 0xe8:rd:t1:t8 0xe8:rt:t0:t7 0xec:rd:t2:t9 0xec:rs:a3:a1 0xec:rt:t1:t8 0xf0:rs:t2:t9 0xf0:rt:t3:t1 0xf4:rt:t6:t4 0xf8:rd:t5:t3 0xf8:rs:t3:t1 0xf8:rt:t4:t2 0xfc:rt:t5:t3 0x100:rs:t6:t4 0x100:rt:t6:t4 0x104:rt:t0:t8 0x108:rd:t7:t5 0x108:rt:t6:t4 0x10c:rd:t8:t6 0x10c:rs:a3:a1 0x10c:rt:t7:t5 0x110:rs:t8:t6 0x110:rt:t9:t7 0x114:rt:t2:t1 0x118:rd:t1:t9 0x118:rs:t9:t7 0x118:rt:t0:t8 0x11c:rt:t1:t9 0x120:rs:t2:t1 0x120:rt:t2:t1 0x124:rt:t6:t5 0x128:rd:t3:t2 0x128:rt:t2:t1 0x12c:rd:t4:t3 0x12c:rs:a3:a1 0x12c:rt:t3:t2 0x130:rs:t4:t3 0x130:rt:t5:t4 0x134:rt:t8:t7 0x138:rd:t7:t6 0x138:rs:t5:t4 0x138:rt:t6:t5 0x13c:rt:t7:t6 0x140:rs:t8:t7 0x140:rt:t8:t7 0x148:rd:t9:t8 0x148:rt:t8:t7 0x14c:rd:t0:t9 0x14c:rs:a3:a1 0x14c:rt:t9:t8 0x150:rs:t0:t9 */
#include "types.h"

extern volatile s32 D_800E9600;
extern s16 D_800B0860[];
extern s32 D_800B2580;

extern u8 *func_80019D00(s32 size, s32 idx);
extern void func_80067628(u8 *buf);
extern void func_800675A8(u8 *buf, s32 arg);
extern void func_800674F4(u8 *ptr, u8 *buf);

void func_8013BA4C(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4,
                   s32 p5, u32 p6, u32 p7, s32 p8, s32 p9) {
    u8 *buf;
    u32 p7h;
    u32 p7m;
    u32 p6h;
    u32 p6m;

    buf = func_80019D00(28, D_800E9600);
    func_80067628(buf);
    func_800675A8(buf, p9);

    p7h = p7 >> 16;
    p7m = p7 >> 8;
    p6h = p6 >> 16;
    p6m = p6 >> 8;
    buf[5] = (u8)p6m;
    buf[4] = (u8)p6h;
    buf[6] = (u8)p6;
    buf[13] = (u8)p7m;
    buf[12] = (u8)p7h;
    buf[14] = (u8)p7;
    buf[21] = (u8)p7m;
    buf[20] = (u8)p7h;
    buf[22] = (u8)p7;

    *(s16 *)(buf + 8) = D_800B0860[D_800E9600 + 10] + p0;
    *(s16 *)(buf + 10) = D_800B0860[D_800E9600 + 12] + p1;
    *(s16 *)(buf + 16) = D_800B0860[D_800E9600 + 10] + p2;
    *(s16 *)(buf + 18) = D_800B0860[D_800E9600 + 12] + p3;
    *(s16 *)(buf + 24) = D_800B0860[D_800E9600 + 10] + p4;
    *(s16 *)(buf + 26) = D_800B0860[D_800E9600 + 12] + p5;

    func_800674F4((u8 *)&D_800B2580 + D_800E9600 * 64 + p8 * 4, buf);
}
