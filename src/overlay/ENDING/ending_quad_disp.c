/* ending_quad_disp - ENDING overlay, Address: 0x80139B68, Size: 476 bytes */
/* @CompileFlags: -Wo,-varref,16 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: patch_insn_reg_field 0x54:rt:v1:a0 0x58:rt:a2:a1 0x5c:rt:v1:a0 0x5c:rd:a1:t6 0x60:rt:v1:a0 0x60:rd:a0:t7 0x64:rs:a2:a1 0x64:rt:a0:t7 0x68:rs:a2:a1 0x68:rt:a1:t6 0x6c:rs:a2:a1 0x6c:rt:v1:a0 0x70:rs:a2:a1 0x70:rt:a0:t7 0x74:rs:a2:a1 0x74:rt:a1:t6 0x78:rs:a2:a1 0x78:rt:v1:a0 0x7c:rs:a2:a1 0x7c:rt:a0:t7 0x80:rs:a2:a1 0x80:rt:a1:t6 0x84:rs:a2:a1 0x84:rt:v1:a0 0x88:rs:a2:a1 0x88:rt:a1:t6 0x98:rt:t6:t8 0x9c:rs:a2:a1 0x9c:rt:v1:a0 0xa0:rs:a2:a1 0xa0:rt:a0:t7 0xa4:rs:t6:t8 0xa4:rt:t6:t8 0xb0:rt:t6:t8 0xb0:rd:t7:t9 0xb4:rt:t7:t9 0xb4:rd:t8:t2 0xb8:rs:t8:t2 0xb8:rt:t9:t3 0xbc:rt:t3:t5 0xc0:rs:t9:t3 0xc0:rd:t2:t4 0xc4:rs:a2:a1 0xc4:rt:t2:t4 0xc8:rs:t3:t5 0xc8:rt:t3:t5 0xcc:rt:t8:t2 0xd0:rt:t3:t5 0xd0:rd:t4:t6 0xd4:rt:t4:t6 0xd4:rd:t5:t7 0xd8:rs:t5:t7 0xd8:rt:t6:t8 0xe0:rs:t6:t8 0xe0:rd:t7:t9 0xe4:rs:a2:a1 0xe4:rt:t7:t9 0xe8:rs:t8:t2 0xe8:rt:t8:t2 0xec:rt:t6:t8 0xf0:rt:t8:t2 0xf0:rd:t9:t3 0xf4:rt:t9:t3 0xf4:rd:t2:t4 0xf8:rs:t2:t4 0xf8:rt:t3:t5 0x100:rs:t3:t5 0x100:rd:t4:t6 0x104:rs:t4:t6 0x104:rd:t5:t7 0x108:rs:a2:a1 0x108:rt:t5:t7 0x10c:rs:t6:t8 0x10c:rt:t6:t8 0x110:rt:t3:t5 0x114:rt:t6:t8 0x114:rd:t7:t9 0x118:rt:t7:t9 0x118:rd:t8:t2 0x11c:rs:t8:t2 0x11c:rt:t9:t3 0x120:rt:t8:t2 0x124:rs:t9:t3 0x124:rd:t2:t4 0x128:rs:a2:a1 0x128:rt:t2:t4 0x12c:rs:t3:t5 0x12c:rt:t3:t5 0x134:rt:t3:t5 0x134:rd:t4:t6 0x138:rt:t4:t6 0x138:rd:t5:t7 0x13c:rs:t5:t7 0x13c:rt:t6:t8 0x144:rs:t6:t8 0x144:rd:t7:t9 0x148:rs:a2:a1 0x148:rt:t7:t9 0x14c:rs:t8:t2 0x14c:rt:t8:t2 0x150:rt:t6:t8 0x154:rt:t8:t2 0x154:rd:t9:t3 0x158:rt:t9:t3 0x158:rd:t2:t4 0x15c:rs:t2:t4 0x15c:rt:t3:t5 0x164:rs:t3:t5 0x164:rd:t4:t6 0x168:rs:t4:t6 0x168:rd:t5:t7 0x16c:rs:a2:a1 0x16c:rt:t5:t7 0x170:rs:t6:t8 0x170:rt:t6:t8 0x174:rt:t4:t6 0x178:rt:t6:t8 0x178:rd:t7:t9 0x17c:rt:t7:t9 0x17c:rd:t8:t2 0x180:rs:t8:t2 0x180:rt:t9:t3 0x188:rs:t9:t3 0x188:rd:t2:t4 0x18c:rs:t2:t4 0x18c:rd:t3:t5 0x190:rs:a2:a1 0x190:rt:t3:t5 0x194:rs:t4:t6 0x194:rt:t4:t6 0x198:rt:t2:t4 0x19c:rt:t4:t6 0x19c:rd:t5:t7 0x1a0:rt:t5:t7 0x1a0:rd:t6:t8 0x1a4:rs:t6:t8 0x1a4:rt:t7:t9 0x1ac:rs:t7:t9 0x1ac:rd:t8:t2 0x1b0:rs:t8:t2 0x1b0:rd:t9:t3 0x1b4:rs:a2:a1 0x1b4:rt:t9:t3 0x1b8:rt:t4:t6 0x1bc:rs:t2:t4 0x1bc:rt:t2:t4 0x1c0:rt:t7:t9 0x1c4:rt:t4:t6 0x1c4:rd:t5:t7 0x1c8:rt:t2:t4 0x1c8:rd:t3:t5 0x1cc:rs:t3:t5 0x1cc:rt:t5:t7 0x1cc:rd:t6:t8 0x1d0:rs:t7:t9 0x1d0:rt:t7:t9 0x1d8:rs:t6:t8 0x1d8:rt:t7:t9 */
#include "types.h"

extern u8 *prim_alloc(s32 a0, s32 a1);
extern void SetPolyG4(u8 *a0);
extern void SetSemiTrans(u8 *a0, s32 a1);
extern void AddPrim(u8 *a0);

extern volatile s32 activeBuff;
extern u8 screen_w[];
extern u8 ot_buff0[];

void ending_quad_disp(s32 x, s32 y, s32 w, s32 h, u32 color, s32 col, s32 flag)
{
    s32 pad;
    u8 *spr;
    u8 hi, mid;

    spr = prim_alloc(36, activeBuff);
    SetPolyG4(spr);
    SetSemiTrans(spr, flag);

    spr[5] = color >> 8;
    spr[4] = color >> 16;
    spr[6] = color;
    spr[13] = color >> 8;
    spr[12] = color >> 16;
    spr[14] = color;
    spr[21] = color >> 8;
    spr[20] = color >> 16;
    spr[22] = color;
    spr[28] = color >> 16;
    spr[30] = color;
    spr[29] = color >> 8;

    *(s16 *)(spr + 8) = *(s16 *)(screen_w + activeBuff * 2 + 20) + x;
    *(s16 *)(spr + 10) = *(s16 *)(screen_w + activeBuff * 2 + 24) + y;
    *(s16 *)(spr + 16) = *(s16 *)(screen_w + activeBuff * 2 + 20) + x + w;
    *(s16 *)(spr + 18) = *(s16 *)(screen_w + activeBuff * 2 + 24) + y;
    *(s16 *)(spr + 24) = *(s16 *)(screen_w + activeBuff * 2 + 20) + x;
    *(s16 *)(spr + 26) = *(s16 *)(screen_w + activeBuff * 2 + 24) + y + h;
    *(s16 *)(spr + 32) = *(s16 *)(screen_w + activeBuff * 2 + 20) + x + w;
    *(s16 *)(spr + 34) = *(s16 *)(screen_w + activeBuff * 2 + 24) + y + h;

    AddPrim(ot_buff0 + (activeBuff << 10) + col * 4);
}
