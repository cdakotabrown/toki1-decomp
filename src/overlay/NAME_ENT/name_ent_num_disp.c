/* @PostProc: patch_insn_reg_field 0x7c:rt:a3:v1 0x80:rs:a3:v1 0x80:rt:a3:v1 0x88:rs:a3:v1 0x8c:rt:v1:t2 0x90:rt:t2:t3 0x94:rt:t5:t6 0x98:rd:t3:t4 0x98:rs:v1:t2 0x98:rt:t2:t3 0x9c:rs:t3:t4 0x9c:rt:t4:t5 0xa0:rt:t4:t5 0xa4:rs:t5:t6 0xa4:rt:t5:t6 0xa8:rt:t8:t0 0xac:rd:t6:t7 0xac:rt:t5:t6 0xb0:rd:t7:t8 0xb0:rs:a3:v1 0xb0:rt:t6:t7 0xb4:rs:t7:t8 0xb4:rt:a2:t9 0xbc:rd:t9:t1 0xbc:rs:a2:t9 0xbc:rt:t8:t0 0xc0:rt:t9:t1 0xc4:rt:t0:t2 0xc8:rt:t3:t5 0xcc:rs:t0:t2 0xd0:rs:t3:t5 0xd0:rt:t3:t5 0xd4:rt:t6:t9 0xd8:rt:t7:t8 0xe4:rd:t1:t3 0xe8:rd:t2:t4 0xe8:rt:t1:t3 0xec:rs:t2:t4 0xec:rt:t3:t5 0xf0:rt:t4:t6 0xf8:rt:t4:t6 0xfc:rt:t5:t7 0x100:rt:t6:t9 0x104:rt:t7:t8 0x10c:rt:t5:t7 0x114:rt:t8:t0 0x118:rs:t8:t0 0x118:rt:t8:t0 0x11c:rt:t1:t3 0x120:rd:t9:t1 0x120:rt:t8:t0 0x124:rs:t9:t1 0x124:rt:t0:t2 0x128:rs:t1:t3 0x128:rt:t1:t3 0x12c:rs:t0:t2 0x12c:rt:t1:t3 0x160:rt:t2:t4 0x164:rs:t2:t4 0x164:rt:t2:t4 0x168:rt:t3:t5 0x16c:rt:t2:t4 0x170:rs:t3:t5 0x170:rt:t3:t5 0x174:rt:t4:t6 0x178:rt:t3:t5 0x17c:rs:t4:t6 0x17c:rt:t4:t6 0x180:rt:t5:t7 0x184:rt:t4:t6 0x188:rs:t5:t7 0x188:rt:t5:t7 0x18c:rt:a3:v1 0x190:rs:a3:v1 0x190:rt:a3:v1 0x194:rd:t6:t8 0x194:rt:t5:t7 0x198:rd:t7:t9 0x198:rs:a3:v1 0x198:rt:t6:t8 0x19c:rs:t7:t9 0x19c:rt:v1:t0 0x1a0:rt:t8:t1 0x1a4:rt:t0:t3 0x1a8:rd:t9:t2 0x1a8:rs:v1:t0 0x1a8:rt:t8:t1 0x1ac:rt:t9:t2 0x1b0:rs:t0:t3 0x1b0:rt:t0:t3 0x1b4:rt:t3:t7 0x1b8:rd:t1:t4 0x1b8:rt:t0:t3 0x1bc:rd:t2:t5 0x1bc:rs:a3:v1 0x1bc:rt:t1:t4 0x1c0:rs:t2:t5 0x1c0:rt:a2:t6 0x1c8:rd:t4:t8 0x1c8:rs:a2:t6 0x1c8:rt:t3:t7 0x1cc:rt:t4:t8 0x1d0:rt:t5:t9 0x1d4:rt:t8:t2 0x1d8:rs:t5:t9 0x1dc:rs:t8:t2 0x1dc:rt:t8:t2 0x1e0:rt:t1:t6 0x1e4:rt:t2:t5 0x1f0:rd:t6:t0 0x1f4:rd:t7:t1 0x1f4:rt:t6:t0 0x1f8:rs:t7:t1 0x1f8:rt:t8:t2 0x1fc:rt:t9:t3 0x204:rt:t9:t3 0x208:rt:t0:t4 0x20c:rt:t1:t6 0x210:rt:t2:t5 0x218:rt:t0:t4 0x220:rt:t3:t7 0x224:rs:t3:t7 0x224:rt:t3:t7 0x228:rt:t6:t0 0x22c:rd:t4:t8 0x22c:rt:t3:t7 0x230:rs:t4:t8 0x230:rt:t5:t9 0x234:rs:t6:t0 0x234:rt:t6:t0 0x238:rs:t5:t9 0x238:rt:t6:t0 */
/* name_ent_num_disp - NAME_ENT overlay, Address: 0x80137D50, Size: 580 bytes */
#include "types.h"

extern volatile u32 activeBuff;
extern volatile u8 screen_bri;
extern volatile s16 screen_w[];
extern volatile u8 name_ent_num_uv_tbl[];
extern u8 ot_buff0[];

extern u32 *prim_alloc(s32 type, u32 bufIndex);
extern void SetSprt();
extern void SetSemiTrans();
extern s16 GetClut(s32 type, s32 attr);
extern void AddPrim();

typedef struct {
    u32 word0;
    u8 r, g, b, pad;
    s16 x, y;
    u8 u, v;
    s16 clut;
    s16 w, h;
} SpritePrim;

void name_ent_num_disp(s32 index, s32 yOffset, s32 xOffset)
{
    u32 bufIndex;
    s16 baseX, baseY;
    SpritePrim *prim;
    s16 *posTable;
    u32 *dest;
    s32 tableIndex;
    u8 *uvTable;

    bufIndex = activeBuff;
    prim = (SpritePrim *)prim_alloc(20, bufIndex);
    SetSprt((u32 *)prim);
    SetSemiTrans((u32 *)prim, 0);

    prim->r = screen_bri;
    prim->g = screen_bri;
    prim->b = screen_bri;

    posTable = screen_w;
    baseX = posTable[activeBuff + 10];
    prim->x = baseX + yOffset - 8;

    baseY = posTable[activeBuff + 12];
    prim->y = baseY + xOffset;

    tableIndex = index / 10;
    uvTable = name_ent_num_uv_tbl;
    prim->u = uvTable[tableIndex * 2];
    prim->v = uvTable[tableIndex * 2 + 1];
    prim->h = 16;
    prim->w = 8;

    prim->clut = GetClut(16, 496);

    dest = (u32 *)&ot_buff0[(activeBuff << 10) + 8];
    AddPrim(dest, (u32 *)prim);

    bufIndex = activeBuff;
    prim = (SpritePrim *)prim_alloc(20, bufIndex);
    SetSprt((u32 *)prim);
    SetSemiTrans((u32 *)prim, 0);

    prim->r = screen_bri;
    prim->g = screen_bri;
    prim->b = screen_bri;

    posTable = screen_w;
    baseX = posTable[activeBuff + 10];
    prim->x = baseX + yOffset;

    baseY = posTable[activeBuff + 12];
    prim->y = baseY + xOffset;

    tableIndex = index % 10;
    uvTable = name_ent_num_uv_tbl;
    prim->u = uvTable[tableIndex * 2];
    prim->v = uvTable[tableIndex * 2 + 1];
    prim->h = 16;
    prim->w = 8;

    prim->clut = GetClut(16, 496);

    dest = (u32 *)&ot_buff0[(activeBuff << 10) + 8];
    AddPrim(dest, (u32 *)prim);
}
