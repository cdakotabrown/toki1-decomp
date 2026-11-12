/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x204 */
/* @PostProc: patch_insn_reg_field 0xfc:rt:t7:a0 0x100:rs:t7:a0 0x100:rt:t8:t7 0x104:rd:t9:t8 0x104:rt:t8:t7 0x108:rt:t9:t8 0x10c:rt:t0:t9 0x114:rs:t0:t9 0x114:rt:t1:t0 0x118:rt:t1:t0 0x11c:rt:t2:t1 0x124:rt:t2:t1 0x12c:rt:t3:t2 0x130:rs:t3:t2 0x138:rt:t5:t4 0x13c:rs:t5:t4 0x13c:rt:t5:t4 0x140:rd:t4:t3 0x140:rt:t3:t2 0x144:rs:t4:t3 0x144:rt:t5:t4 0x148:rt:t6:t5 0x150:rs:t6:t5 0x150:rt:t7:t6 0x154:rt:t7:t6 0x158:rt:t8:t7 0x160:rt:t8:t7 0x16c:rt:t9:t8 0x170:rs:t9:t8 0x170:rt:t9:t8 0x174:rt:t1:t0 0x178:rs:t9:t8 0x178:rt:t0:t9 0x17c:rt:t0:t9 0x180:rs:t1:t0 0x180:rt:t1:t0 0x188:rt:t1:t0 0x194:rt:t2:t1 0x198:rs:t2:t1 0x198:rt:t2:t1 0x19c:rt:t4:t3 0x1a0:rs:t2:t1 0x1a0:rt:t3:t2 0x1a4:rt:t3:t2 0x1a8:rs:t4:t3 0x1a8:rt:t4:t3 0x1b0:rt:t4:t3 0x1bc:rt:t8:t7 0x1c0:rt:t5:t4 0x1c4:rs:t5:t4 0x1c4:rt:t5:t4 0x1c8:rt:t7:t6 0x1cc:rs:t5:t4 0x1cc:rt:t6:t5 0x1d0:rt:t6:t5 0x1d4:rs:t7:t6 0x1d4:rt:t7:t6 0x1dc:rt:t7:t6 0x1e0:rs:t8:t7 0x1e0:rt:t8:t7 0x1e4:rt:t0:t9 0x1e8:rs:t8:t7 0x1e8:rt:t9:t8 0x1ec:rt:t9:t8 0x1f0:rs:t0:t9 0x1f0:rt:t0:t9 0x1f8:rt:t0:t9 0x1fc:rt:t1:t0 0x200:rt:t1:t0 0x218:rt:t2:t1 0x21c:rs:t2:t1 0x21c:rt:t2:t1 0x220:rt:t5:t4 0x224:rd:t3:t2 0x224:rt:t2:t1 0x228:rs:t3:t2 0x228:rt:t4:t3 0x22c:rs:t5:t4 0x22c:rt:t5:t4 0x230:rs:t4:t3 0x230:rt:t5:t4 0x23c:rt:t6:t5 0x244:rs:t6:t5 */
/* Address: 0x801431D0 */
/* Size: 644 bytes (161 insns) */
#include "types.h"

typedef struct {
    u8  pad0[4];
    u8  f4;
    u8  f5;
    u8  f6;
    u8  pad1;
    s16 f8;
    s16 f10;
    u8  f12;
    u8  f13;
    s16 f14;
    s16 f16;
    s16 f18;
} Spr;

extern s16 screen_w[];
extern u8 ot_buff0[];
extern s32 activeBuff;
extern u8 taco_glyph_uv_tbl[];
extern u8 taco_glyph_uv_tbl_36;
extern u8 taco_glyph_uv_tbl_36_v;
extern u8 taco_glyph_uv_tbl_37;
extern u8 taco_glyph_uv_tbl_37_v;
extern u8 taco_glyph_uv_tbl_38;
extern u8 taco_glyph_uv_tbl_38_v;
extern u8 taco_glyph_uv_tbl_39;
extern u8 taco_glyph_uv_tbl_39_v;
extern u8 taco_glyph_uv_tbl_10[];

extern s32 prim_alloc(s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern s32 GetClut(s32, s32);
extern void AddPrim(s32, s32);
extern void SetSemiTrans(s32, s32);
extern void SetSprt(s32);

void taco_str_disp2(u8 *str, s32 x, s32 y, s32 kind)
{
    Spr *p;
    u8 *s;
    u32 c;
    s32 xx;

    s = str;
    xx = x;
    if (*s != 0) {
        do {
            p = (Spr *)prim_alloc(20, activeBuff);
            SetSprt((s32)p);
            SetSemiTrans((s32)p, 0);
            p->f4 = 128;
            p->f5 = 128;
            p->f6 = 128;
            p->f8 = (s16)(screen_w[activeBuff + 10] + xx);
            p->f10 = (s16)(screen_w[activeBuff + 12] + y);
            xx += 12;
            c = *s;
            if (c >= 48 && c < 58) {
                p->f12 = (u8)(taco_glyph_uv_tbl[(u8)(c - 48) * 2] + 128);
                p->f13 = taco_glyph_uv_tbl[(u8)(c - 48) * 2 + 1];
            } else if ((u8)(c - 65) < 26U) {
                p->f12 = (u8)(taco_glyph_uv_tbl_10[(u8)(c - 65) * 2] + 128);
                p->f13 = taco_glyph_uv_tbl_10[(u8)(c - 65) * 2 + 1];
            } else if (c == 46) {
                p->f12 = (u8)(taco_glyph_uv_tbl_36 + 128);
                p->f13 = taco_glyph_uv_tbl_36_v;
            } else if (c == 45) {
                p->f12 = (u8)(taco_glyph_uv_tbl_37 + 128);
                p->f13 = taco_glyph_uv_tbl_37_v;
            } else if (c == 32) {
                p->f12 = (u8)(taco_glyph_uv_tbl_38 + 128);
                p->f13 = taco_glyph_uv_tbl_38_v;
            } else {
                p->f12 = (u8)(taco_glyph_uv_tbl_39 + 128);
                p->f13 = taco_glyph_uv_tbl_39_v;
            }
            p->f16 = 12;
            p->f18 = 16;
            p->f14 = (s16)GetClut(kind << 4, 495);
            AddPrim((s32)(ot_buff0 + (activeBuff << 10) + 4), (s32)p);
        } while (*++s != 0);
    }

    dtd_on_tpage(832, 384, 1, 0, 0);
}
