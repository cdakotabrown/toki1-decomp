/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=1,2 IDO52_UGEN_TEMP_REG=9:t7 IDO52_FORCE_WEB_COLOR=62:t1,123:t0,135:a3,117:v1,146:a0,152:v1 */
/* @SymbolSize: 712 */
/* @PostProc: patch_insn_reg_field 0xa4:rd:t6:t8 0xa8:rt:t8:t6 0xac:rt:t3:t9 0xb4:rt:t9:t2 0xb8:rt:t7:t4 0xbc:rd:t2:t3 0xbc:rt:t9:t2 0xc0:rs:t2:t9 0xc4:rt:t2:t9 0xc8:rt:t4:t5 0xd8:rt:t2:t9 0xe0:rd:t5:t7 0xe0:rt:t4:t5 0xe8:rt:t4:t5 0xec:rs:t5:t4 0xf0:rd:t9:t2 0xf4:rt:t9:t2 0xf8:rs:t4:t5 0x100:rt:t5:t4 0x108:rt:t5:t4 0x124:rs:t8:s4 0x124:rt:s4:t8 0x150:rs:t9:t2 0x150:rt:t2:t9 0x160:rt:t4:t5 0x164:rs:t4:t5 0x168:rt:t9:t2 0x16c:rt:t5:t4 0x178:rs:t5:t4 0x194:rs:t9:t2 0x19c:rt:t2:t9 0x1a8:rs:t2:t9 0x1b4:rt:t4:t5 0x1b8:rt:t5:t4 0x1c0:rs:t4:t5 0x1c0:rt:t5:t4 0x1d4:rd:t7:a2 0x1d8:rs:t7:t1 0x1d8:rt:t1:a2 0x1dc:rt:t8:t7 0x1e4:rt:t9:t6 0x1e8:rt:t9:t6 0x1ec:rs:t8:t7 0x1ec:rt:t6:t8 0x1f0:rd:t3:t9 0x1f0:rs:t6:t8 0x1f4:rt:t3:t9 0x1f8:rt:t4:t3 0x1fc:rd:t8:t7 0x200:rs:t4:t3 0x204:rd:t7:t4 0x208:rd:t9:t6 0x208:rt:t8:t7 0x20c:rd:t6:t8 0x20c:rs:t7:t4 0x20c:rt:t9:t6 0x210:rt:t6:t8 0x224:rt:t3:t9 0x228:rt:t3:t9 0x22c:rt:t4:t3 0x234:rs:t4:t3 0x23c:rt:v1:v0 0x244:rs:v1:v0 0x248:rs:v1:v0 0x248:rt:t7:t4 0x24c:rd:t9:t6 0x24c:rt:t7:t4 0x250:rd:t8:t7 0x250:rs:t5:s5 0x250:rt:s5:t5 0x254:rd:t6:t8 0x254:rs:t8:t7 0x254:rt:t9:t6 0x25c:rt:t6:t8 0x260:rt:v1:v0 0x268:rs:v1:v0 0x26c:rs:v1:v0 0x26c:rt:t4:t3 0x270:rt:t4:t3 0x274:rd:t3:t9 0x274:rs:t2:s5 0x274:rt:s5:t2 0x278:rd:t7:t4 0x278:rs:t3:t9 0x27c:rt:t7:t4 0x284:rt:t8:t7 0x28c:rt:t8:t7 */
/* anim_sprite_disp - TT overlay, Address: 0x80147A08, Size: 712 bytes (178 insns) */
#include "types.h"

extern void tt_sprite_ot_add_16x16(s32, s32, s32);

extern s32 *tt_disp_wk;
extern u8  *tt_db_buf;
extern s16 *tt_scale_buf;

void anim_sprite_disp(s16 *pos, u8 *req, u16 idx)
{
    s32  unused0;
    u8  *ot;
    s16 *scr;
    s32 **lists;
    s32 *entry;
    u8  *cels;
    u16 *ip;
    u8  *cel;
    u8  *prim;
    u8  *pix;
    s32  otslot;
    s32  n;
    s32  x;
    s32  y;
    s32  flags;
    u32  tex;
    u32  lo;
    s32  clut;
    s32  one;
    u8   pal;

    ot = tt_db_buf + tt_disp_wk[2] * 32952;
    lists = *(s32 ***) req;
    scr = tt_scale_buf;

    if (lists == (s32 **) 0) {
        return;
    }

    entry = lists[req[5]];
    ip = (u16 *) ((u8 *) entry[0] + req[6] * 4);
    cels = (u8 *) entry[1] + *ip * 4;
    cel = (u8 *) entry[2] + *(u16 *) (cels + 0) * 8
          + *(u16 *) (cels + 2) * 8 - 8;
    pix = (u8 *) entry[3];
    clut = *(u16 *) ((u8 *) entry + 16);
    scr[2] = pos[0];
    scr[3] = pos[1];

    if (req[16] != 0) {
        scr[0] = *(s16 *) (req + 12);
        scr[1] = *(s16 *) (req + 14);
    }

    n = *(u16 *) cels;
    otslot = (s32) (ot + idx * 4) + 0x8008;
    one = 1;

    for (; n > 0; n--, cel -= 8) {
        prim = *(u8 **) (ot + 4);
        *(u8 **) (ot + 4) = prim + 0x30;

        prim[0x28] = (u8) (cel[7] ^ req[4]);

        flags = req[4];
        if ((flags & 1) != 0) {
            y = pos[1] - *(s16 *) (cel + 4);
            y -= 16;
        } else {
            y = *(s16 *) (cel + 4) + pos[1];
        }
        if ((flags & 2) != 0) {
            x = pos[0] - *(s16 *) (cel + 2);
            x -= 16;
        } else {
            x = *(s16 *) (cel + 2) + pos[0];
        }
        *(s16 *) (prim + 8) = (s16) x;
        *(s16 *) (prim + 10) = (s16) y;

        tex = *(u16 *) cel;
        lo = tex & 0x3F;
        prim[0x0C] = (u8) ((pix[tex >> 6] & 0x80) + ((lo & 7) << 4));
        prim[0x0D] = (u8) (((pix[tex >> 6] & 0x40) << 1)
                           + (((s32) lo >> 3) << 4));
        prim[0x29] = 0;
        prim[0x2A] = (u8) one;
        *(s16 *) (prim + 0x16) = (s16) (pix[tex >> 6] & 0x1F);

        if (req[10] != 0) {
            pal = req[11];
            *(s16 *) (prim + 0x0E) =
                (s16) (clut + (pal & 0xF) + ((pal & 0xF0) << 2));
        } else {
            pal = cel[6];
            *(s16 *) (prim + 0x0E) =
                (s16) (clut + (pal & 0xF) + ((pal & 0xF0) << 2));
        }

        prim[0x2C] = 0;
        prim[0x2B] = req[16];
        tt_sprite_ot_add_16x16(otslot, (s32) prim, (s32) scr);

    }
}
