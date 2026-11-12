/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:64 */
/* @PostProc: patch_insn_reg_field 0x7c:rt:a2:v1 0x80:rs:a2:v1 0x80:rt:a2:v1 0x88:rs:a2:v1 0x8c:rt:v1:t2 0x90:rt:t2:t3 0x94:rt:t4:t5 0x98:rd:t3:t4 0x98:rs:v1:t2 0x98:rt:t2:t3 0x9c:rt:t3:t4 0xa0:rs:t4:t5 0xa0:rt:t4:t5 0xa4:rt:t7:t9 0xa8:rd:t5:t6 0xa8:rt:t4:t5 0xac:rd:t6:t7 0xac:rs:a2:v1 0xac:rt:t5:t6 0xb0:rs:t6:t7 0xb0:rt:a1:t8 0xb4:rt:t1:t3 0xb8:rd:t8:t0 0xb8:rs:a1:t8 0xb8:rt:t7:t9 0xbc:rt:t8:t0 0xc0:rt:t9:t1 0xc4:rs:t1:t3 0xc4:rt:t1:t3 0xc8:rd:t0:t2 0xc8:rt:t9:t1 0xcc:rs:t0:t2 0xcc:rt:t1:t3 0xd0:rt:t2:t4 0xd8:rt:t2:t4 0xdc:rt:t3:t5 0xf0:rt:t3:t5 0xf8:rt:t4:t6 0xfc:rs:t4:t6 0xfc:rt:t4:t6 0x100:rt:t7:t9 0x104:rd:t5:t7 0x104:rt:t4:t6 0x108:rs:t5:t7 0x108:rt:t6:t8 0x10c:rs:t7:t9 0x10c:rt:t7:t9 0x110:rs:t6:t8 0x110:rt:t7:t9 */
/* name_ent_digit_disp - NAME_ENT overlay, Address: 0x80137C34, Size: 284 bytes (71 insns) */
#include "types.h"

extern u32 activeBuff;
extern u8 screen_bri;
extern s16 screen_w[];
extern u8 name_ent_digit_uv_tbl[];
extern u8 *ot_buff0;

extern u32 *prim_alloc(s32 type, u32 bufIndex);
extern void SetSprt();
extern void SetSemiTrans();
extern s16 GetClut(s32 type, s32 attr);
extern void AddPrim();

typedef struct {
    u32 word0;
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    s16 clut;
    s16 w;
    s16 h;
} SpritePrim;

void name_ent_digit_disp(s32 index, s32 yOffset, s32 xOffset)
{
    u32 bufIndex;
    u8 size;
    s16 baseX, baseY;
    s16 *posTable;
    u8 *uvTable;
    u32 *dest;
    SpritePrim *prim;

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

    uvTable = name_ent_digit_uv_tbl;
    prim->u = uvTable[index * 2];
    prim->v = uvTable[index * 2 + 1];

    size = 16;
    prim->h = size;
    prim->w = size;

    prim->clut = GetClut(size, 496);

    dest = (u32 *)((activeBuff << 10) + 8 + (u32)&ot_buff0);
    AddPrim(dest, (u32 *)prim);
}
