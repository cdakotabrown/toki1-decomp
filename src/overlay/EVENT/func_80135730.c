/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --load-fp --branch-fp --fence 103-115 */
/* @PostProc: patch_insn_reg_field 0x54:rt:v0:a0 0x58:rt:a1:t2 0x5c:rt:a2:a1 0x60:rd:v1:t6 0x60:rt:v0:a0 0x64:rd:a0:t7 0x64:rt:v0:a0 0x68:rd:a3:t8 0x68:rt:a1:t2 0x6c:rd:t0:t9 0x6c:rt:a1:t2 0x74:rs:a2:a1 0x74:rt:a0:t7 0x78:rs:a2:a1 0x78:rt:v1:t6 0x7c:rs:a2:a1 0x7c:rt:v0:a0 0x80:rs:a2:a1 0x80:rt:a0:t7 0x84:rs:a2:a1 0x84:rt:v1:t6 0x88:rs:a2:a1 0x88:rt:v0:a0 0x8c:rs:a2:a1 0x8c:rt:t0:t9 0x90:rs:a2:a1 0x90:rt:a3:t8 0x94:rs:a2:a1 0x94:rt:a1:t2 0x98:rs:a2:a1 0x98:rt:t0:t9 0x9c:rs:a2:a1 0x9c:rt:a3:t8 0xa0:rs:a2:a1 0xa0:rt:a1:t2 0xbc:rt:t1:t7 0xc8:rs:a2:a1 0xc8:rt:t2:t8 0xd0:rt:t7:t6 0xd4:rd:t1:t3 0xd8:rd:t2:t4 0xd8:rs:t9:v0 0xd8:rt:t1:t3 0xdc:rs:t5:t4 0xdc:rt:t6:t5 0xe0:rt:t3:t8 0xe4:rd:t2:t7 0xe4:rs:v0:t5 0xe4:rt:t1:t6 0xe8:rs:a2:a1 0xe8:rt:t8:t7 0xec:rs:t3:t8 0xec:rt:t3:t8 0xf0:rt:t4:t5 0xf4:rd:t4:t9 0xf4:rt:t3:t8 0xf8:rd:t2:t3 0xf8:rs:t9:v0 0xf8:rt:t1:t9 0xfc:rs:t2:t3 0xfc:rt:t3:t4 0x100:rt:t9:t7 0x104:rd:t5:t6 0x104:rs:v0:t4 0x104:rt:t4:t5 0x108:rs:a2:a1 0x108:rt:t5:t6 0x10c:rs:t9:t7 0x10c:rt:t9:t7 0x110:rt:t1:t4 0x114:rd:t1:t8 0x114:rt:t9:t7 0x118:rd:t8:t9 0x118:rs:t6:v0 0x118:rt:t7:t8 0x11c:rs:t8:t9 0x11c:rt:t9:t3 0x128:rs:a2:a1 0x128:rt:t2:t5 0x130:rt:t7:t3 0x140:rt:t6:t5 0x144:rd:t2:t4 0x144:rs:v0:t9 0x144:rt:t1:t3 0x148:rs:a2:a1 0x148:rt:t8:t4 0x14c:rs:t6:t5 0x14c:rt:t6:t5 0x150:rt:t4:t9 0x154:rd:t7:t6 0x154:rt:t6:t5 0x158:rd:t5:t7 0x158:rs:t3:v0 0x158:rt:t4:t6 0x15c:rs:t2:t7 0x15c:rt:t3:t8 0x160:rt:t6:t4 0x164:rd:t8:t3 0x164:rs:v0:t8 0x164:rt:t7:t9 0x168:rs:a2:a1 0x168:rt:t5:t3 0x16c:rs:t6:t4 0x16c:rt:t6:t4 0x170:rt:t1:t8 0x174:rd:t7:t5 0x174:rt:t6:t4 0x178:rd:t2:t6 0x178:rs:t9:v0 0x178:rt:t1:t5 0x17c:rs:t8:t6 0x17c:rt:t9:t7 0x184:rd:t8:t9 0x184:rs:v0:t7 0x184:rt:t7:t8 0x188:rs:a2:a1 0x188:rt:t2:t9 0x1a8:rs:a2:a1 0x1ac:rt:t2:t4 0x1b4:rt:t5:t7 0x1b8:rd:t3:t5 0x1b8:rt:t2:t4 0x1bc:rd:t1:t3 0x1c0:rd:t4:t6 0x1c0:rs:t1:t3 0x1c0:rt:t3:t5 0x1c4:rs:t5:t7 0x1c4:rt:t5:t7 0x1cc:rs:t4:t6 0x1cc:rt:t5:t7 */
/* @SymbolSize: 464 */
#include "types.h"

extern u8 *func_80019D00(s32 size, s32 idx);
extern void func_80067678(u8 *spr);
extern void func_800675A8(u8 *spr, s32 arg);
extern void func_800674F4(s32 *entry);

extern volatile s32 D_800E9600;
extern s16 D_800B0860[];
extern s32 D_800B2580[];

void func_80135730(s32 p1, s32 p2, s32 p3, s32 p4, s32 p5, s32 p6,
                   s32 p7, s32 p8, u32 p9, u32 p10, s32 p11, s32 p12)
{
    u8 *spr;
    u8 p9hi, p9mid, p10hi, p10mid;

    spr = func_80019D00(36, D_800E9600);
    func_80067678(spr);
    func_800675A8(spr, p12);

    p9hi = (u8)(p9 >> 16);
    p9mid = (u8)(p9 >> 8);
    p10hi = (u8)(p10 >> 16);
    p10mid = (u8)(p10 >> 8);

    spr[5] = p9mid;
    spr[4] = p9hi;
    spr[6] = (u8)p9;
    spr[13] = p9mid;
    spr[12] = p9hi;
    spr[14] = (u8)p9;
    spr[21] = p10mid;
    spr[20] = p10hi;
    spr[22] = (u8)p10;
    spr[29] = p10mid;
    spr[28] = p10hi;
    spr[30] = (u8)p10;

    *(s16 *)(spr + 8) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 20) + p1;
    *(s16 *)(spr + 10) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 24) + p2;
    *(s16 *)(spr + 16) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 20) + p3;
    *(s16 *)(spr + 18) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 24) + p4;
    *(s16 *)(spr + 24) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 20) + p5;
    *(s16 *)(spr + 26) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 24) + p6;
    *(s16 *)(spr + 32) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 20) + p7;
    *(s16 *)(spr + 34) =
        *(s16 *)((u8 *)D_800B0860 + D_800E9600 * 2 + 24) + p8;

    func_800674F4((s32 *)((u8 *)D_800B2580 + D_800E9600 * 64 + p11 * 4));
}
