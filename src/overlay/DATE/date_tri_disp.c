/* @SymbolSize: 392 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6C */
/* @PostProc: patch_insn_reg_field 0x54:rt:t0:t1 0x58:rt:v1:a0 0x60:rd:a1:t2 0x60:rt:v1:a0 0x64:rd:a0:t3 0x64:rt:v1:a0 0x68:rt:t0:t1 0x6c:rd:t6:t9 0x6c:rt:t0:t1 0x70:rt:t8:t4 0x74:rt:t6:t9 0x7c:rt:t0:t1 0x80:rt:a0:t3 0x84:rt:a1:t2 0x88:rt:v1:a0 0x8c:rt:a0:t3 0x90:rt:a1:t2 0x94:rt:v1:a0 0x98:rs:t8:t4 0x98:rt:t8:t4 0x9c:rt:a3:a1 0xa0:rs:a3:a1 0xa0:rt:a3:a1 0xa4:rd:t9:t5 0xa4:rt:t8:t4 0xa8:rd:t1:t6 0xa8:rs:a3:a1 0xa8:rt:t9:t5 0xac:rs:t1:t6 0xac:rt:t2:t7 0xb0:rt:t3:t8 0xb4:rt:t5:t1 0xb8:rd:t4:t9 0xb8:rs:t2:t7 0xb8:rt:t3:t8 0xbc:rt:t4:t9 0xc0:rs:t5:t1 0xc0:rt:t5:t1 0xc4:rt:t9:t5 0xc8:rd:t6:t2 0xc8:rt:t5:t1 0xcc:rd:t7:t3 0xcc:rs:a3:a1 0xcc:rt:t6:t2 0xd0:rs:t7:t3 0xd0:rt:t8:t4 0xd4:rt:t2:t7 0xd8:rd:t1:t6 0xd8:rs:t8:t4 0xd8:rt:t9:t5 0xdc:rt:t1:t6 0xe0:rs:t2:t7 0xe0:rt:t2:t7 0xe4:rt:t6:t2 0xe8:rd:t3:t8 0xe8:rt:t2:t7 0xec:rd:t4:t9 0xec:rs:a3:a1 0xec:rt:t3:t8 0xf0:rs:t4:t9 0xf0:rt:t5:t1 0xf4:rt:t8:t4 0xf8:rd:t7:t3 0xf8:rs:t5:t1 0xf8:rt:t6:t2 0xfc:rt:t7:t3 0x100:rs:t8:t4 0x100:rt:t8:t4 0x104:rt:t3:t8 0x108:rd:t9:t5 0x108:rt:t8:t4 0x10c:rd:t1:t6 0x10c:rs:a3:a1 0x10c:rt:t9:t5 0x110:rs:t1:t6 0x110:rt:t2:t7 0x114:rt:t5:t1 0x118:rd:t4:t9 0x118:rs:t2:t7 0x118:rt:t3:t8 0x11c:rt:t4:t9 0x120:rs:t5:t1 0x120:rt:t5:t1 0x124:rt:t9:t5 0x128:rd:t6:t2 0x128:rt:t5:t1 0x12c:rd:t7:t3 0x12c:rs:a3:a1 0x12c:rt:t6:t2 0x130:rs:t7:t3 0x130:rt:t8:t4 0x134:rt:t2:t7 0x138:rd:t1:t6 0x138:rs:t8:t4 0x138:rt:t9:t5 0x13c:rt:t1:t6 0x140:rs:t2:t7 0x140:rt:t2:t7 0x144:rt:t6:t2 0x148:rd:t3:t8 0x148:rt:t2:t7 0x14c:rd:t4:t9 0x14c:rs:a3:a1 0x14c:rt:t3:t8 0x150:rs:t4:t9 0x150:rt:t5:t1 0x154:rt:t8:t4 0x158:rd:t7:t3 0x158:rs:t5:t1 0x158:rt:t6:t2 0x15c:rt:t7:t3 0x160:rt:t1:t6 0x164:rs:t8:t4 0x164:rt:t8:t4 0x168:rt:t4:t9 0x16c:rd:t2:t7 0x16c:rt:t1:t6 0x170:rd:t9:t5 0x170:rt:t8:t4 0x174:rd:t3:t8 0x174:rs:t9:t5 0x174:rt:t2:t7 0x178:rs:t4:t9 0x178:rt:t4:t9 0x17c:rs:t3:t8 0x17c:rt:t4:t9 */
/* Address: 0x80152E34, Size: 392 bytes (98 insns) */
#include "types.h"

extern s32 prim_alloc(s32 a0, s32 a1);
extern s32 SetPolyG3(s32 a0);
extern s32 SetSemiTrans(s32 a0, s32 a1);
extern s32 AddPrim(s32 a0, s32 a1);

extern volatile s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];

s32 date_tri_disp(s32 a0, s32 a1, s32 a2, s32 a3,
                  s32 a4, s32 a5, s32 a6, s32 a7,
                  s32 a8, s32 a9)
{
    u8 *buf;
    s32 h;
    s32 a7hi, a7lo, a6hi, a6lo;

    h = prim_alloc(28, activeBuff);
    SetPolyG3(h);
    buf = (u8 *)h;
    SetSemiTrans(h, a9);

    a7hi = (s32)((u32)a7 >> 16);
    a7lo = (s32)((u32)a7 >> 8);
    a6hi = (s32)((u32)a6 >> 16);
    a6lo = (s32)((u32)a6 >> 8);
    buf[5]  = (u8)a6lo;
    buf[4]  = (u8)a6hi;
    buf[6]  = (u8)a6;
    buf[13] = (u8)a7lo;
    buf[12] = (u8)a7hi;
    buf[14] = (u8)a7;
    buf[21] = (u8)a7lo;
    buf[20] = (u8)a7hi;
    buf[22] = (u8)a7;

    *(s16 *)(buf + 8)  = screen_w[activeBuff + 10] + a0;
    *(s16 *)(buf + 10) = screen_w[activeBuff + 12] + a1;
    *(s16 *)(buf + 16) = screen_w[activeBuff + 10] + a2;
    *(s16 *)(buf + 18) = screen_w[activeBuff + 12] + a3;
    *(s16 *)(buf + 24) = screen_w[activeBuff + 10] + a4;
    *(s16 *)(buf + 26) = screen_w[activeBuff + 12] + a5;

    return AddPrim(
        (s32)(&ot_buff0[(activeBuff << 10) + a8 * 4]), (s32)buf);
}
