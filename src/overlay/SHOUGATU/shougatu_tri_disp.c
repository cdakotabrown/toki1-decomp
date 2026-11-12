/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: patch_insn_reg_field 0x54:rt:a1:t1 0x58:rt:v0:a0 0x60:rd:v1:t2 0x60:rt:v0:a0 0x64:rd:a0:t3 0x64:rt:v0:a0 0x68:rd:t6:t7 0x68:rt:a1:t1 0x6c:rd:t7:t9 0x6c:rt:a1:t1 0x70:rt:t8:t4 0x74:rt:t7:t9 0x78:rt:t6:t7 0x7c:rt:a1:t1 0x80:rt:a0:t3 0x84:rt:v1:t2 0x88:rt:v0:a0 0x8c:rt:a0:t3 0x90:rt:v1:t2 0x94:rt:v0:a0 0x98:rs:t8:t4 0x98:rt:t8:t4 0x9c:rt:a3:a1 0xa0:rs:a3:a1 0xa0:rt:a3:a1 0xa4:rd:t9:t5 0xa4:rt:t8:t4 0xa8:rd:t0:t6 0xa8:rs:a3:a1 0xa8:rt:t9:t5 0xac:rs:t0:t6 0xac:rt:t1:t7 0xb0:rt:t2:t8 0xb4:rt:t4:t1 0xb8:rd:t3:t9 0xb8:rs:t1:t7 0xb8:rt:t2:t8 0xbc:rt:t3:t9 0xc0:rs:t4:t1 0xc0:rt:t4:t1 0xc4:rt:t8:t5 0xc8:rd:t5:t2 0xc8:rt:t4:t1 0xcc:rd:t6:t3 0xcc:rs:a3:a1 0xcc:rt:t5:t2 0xd0:rs:t6:t3 0xd0:rt:t7:t4 0xd4:rt:t0:t7 0xd8:rd:t9:t6 0xd8:rs:t7:t4 0xd8:rt:t8:t5 0xdc:rt:t9:t6 0xe0:rs:t0:t7 0xe0:rt:t0:t7 0xe4:rt:t4:t2 0xe8:rd:t1:t8 0xe8:rt:t0:t7 0xec:rd:t2:t9 0xec:rs:a3:a1 0xec:rt:t1:t8 0xf0:rs:t2:t9 0xf0:rt:t3:t1 0xf4:rt:t6:t4 0xf8:rd:t5:t3 0xf8:rs:t3:t1 0xf8:rt:t4:t2 0xfc:rt:t5:t3 0x100:rs:t6:t4 0x100:rt:t6:t4 0x104:rt:t0:t8 0x108:rd:t7:t5 0x108:rt:t6:t4 0x10c:rd:t8:t6 0x10c:rs:a3:a1 0x10c:rt:t7:t5 0x110:rs:t8:t6 0x110:rt:t9:t7 0x114:rt:t2:t1 0x118:rd:t1:t9 0x118:rs:t9:t7 0x118:rt:t0:t8 0x11c:rt:t1:t9 0x120:rs:t2:t1 0x120:rt:t2:t1 0x124:rt:t6:t5 0x128:rd:t3:t2 0x128:rt:t2:t1 0x12c:rd:t4:t3 0x12c:rs:a3:a1 0x12c:rt:t3:t2 0x130:rs:t4:t3 0x130:rt:t5:t4 0x134:rt:t8:t7 0x138:rd:t7:t6 0x138:rs:t5:t4 0x138:rt:t6:t5 0x13c:rt:t7:t6 0x140:rs:t8:t7 0x140:rt:t8:t7 0x148:rd:t9:t8 0x148:rt:t8:t7 0x14c:rd:t0:t9 0x14c:rs:a3:a1 0x14c:rt:t9:t8 0x150:rs:t0:t9 */
/* Address: 0x801405E0 */
/* Size: 392 bytes (98 instructions) */
#include "types.h"

extern volatile u32 activeBuff;
extern s16 screen_w[];
extern u8 *ot_buff0;

extern u32 *prim_alloc(s32 size, u32 bufIndex);
extern void SetPolyG3();
extern void SetSemiTrans();
extern void AddPrim();

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 pad1;
    s16 x1;
    s16 y1;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 pad2;
    s16 x2;
    s16 y2;
} PolyG3;

void shougatu_tri_disp(s32 x0Off, s32 y0Off, s32 x1Off, s32 y1Off, s32 x2Off,
                   s32 y2Off, u32 col0, u32 col12, s32 otSlot, s32 arg10)
{
    PolyG3 *prim;
    s16 *posTable;

    prim = (PolyG3 *)prim_alloc(28, activeBuff);
    SetPolyG3(prim);
    SetSemiTrans(prim, arg10);

    prim->r0 = col0 >> 16;
    prim->g0 = col0 >> 8;
    prim->b0 = col0;
    prim->r1 = col12 >> 16;
    prim->g1 = col12 >> 8;
    prim->b1 = col12;
    prim->r2 = col12 >> 16;
    prim->g2 = col12 >> 8;
    prim->b2 = col12;

    posTable = screen_w;
    prim->x0 = posTable[activeBuff + 10] + x0Off;
    prim->y0 = posTable[activeBuff + 12] + y0Off;
    prim->x1 = posTable[activeBuff + 10] + x1Off;
    prim->y1 = posTable[activeBuff + 12] + y1Off;
    prim->x2 = posTable[activeBuff + 10] + x2Off;
    prim->y2 = posTable[activeBuff + 12] + y2Off;

    AddPrim((u32 *)((activeBuff << 10) + (otSlot << 2) +
                          (u32)&ot_buff0), prim);
}
