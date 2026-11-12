/* tt_serve_walk_step - TT overlay, Address: 0x80138808, Size: 904 bytes (226 insns) */
/* - `@CompileFlags: -Wo,-loopunroll,2`.  The units-16..22 loop is the */
/* @CompileFlags: -Wo,-loopunroll,2 */
/* @SymbolSize: 904 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x16c */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1c4 */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1cc --offset2 0x1d0 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d4 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t2:t5 0x3c:rs:t2:t5 0x3c:rt:t2:t5 0x5c:rt:t4:t9 0x6c:rt:t0:t1 0x70:rt:t1:a3 0x74:rt:t0:t1 0x7c:rt:t1:a3 0x9c:rt:a2:t2 0xa0:rs:a2:t2 0xa0:rt:a2:t2 0xb0:rd:s0:a2 0xb4:rs:t2:t5 0xb8:rt:a3:t0 0xbc:rt:a2:t2 0xc4:rt:t1:a3 0xd4:rt:a3:t0 0xe0:rs:s0:a2 0xe0:rt:s0:a2 0xe4:rd:t3:t6 0xe8:rt:t3:t6 0xec:rt:t5:t8 0xf0:rt:t4:t7 0xf4:rs:s0:a2 0xf8:rd:t6:t9 0xf8:rs:t4:t7 0xf8:rt:t5:t8 0xfc:rs:t6:t9 0xfc:rt:t7:t6 0x100:rt:t7:t6 0x118:rt:t0:t1 0x11c:rt:a2:t2 0x124:rt:t1:a3 0x134:rt:a3:t0 0x138:rs:t2:t5 0x138:rt:v1:t7 0x13c:rt:a0:t8 0x144:rd:t8:t9 0x144:rs:v1:t7 0x144:rt:a0:t8 0x148:rt:t8:t9 0x14c:rs:t2:t5 0x14c:rt:v1:t6 0x150:rt:a0:t7 0x154:rt:t0:t1 0x158:rd:t9:t8 0x158:rs:v1:t6 0x158:rt:a0:t7 0x15c:rs:t9:t8 0x15c:rt:t3:t9 0x160:rt:t3:t9 0x170:rs:t2:t5 0x174:rt:t2:a2 0x178:rt:a2:t2 0x180:rt:t1:a3 0x190:rt:a3:t0 0x194:rt:t5:t7 0x198:rt:t4:t6 0x1a0:rd:t6:t8 0x1a0:rs:t4:t6 0x1a0:rt:t5:t7 0x1a4:rt:t6:t8 0x1a8:rt:t8:t6 0x1ac:rt:t7:t9 0x1b4:rd:t9:t7 0x1b4:rs:t7:t9 0x1b4:rt:t8:t6 0x1b8:rs:t9:t7 0x1b8:rt:t3:t8 0x1bc:rt:t3:t8 0x1c8:rt:t1:a3 0x1d0:rt:a2:t2 0x1d4:rt:t0:t1 0x1dc:rt:a3:t0 0x1e0:rt:t5:t6 0x1e4:rt:t4:t9 0x1ec:rd:t6:t7 0x1ec:rs:t4:t9 0x1ec:rt:t5:t6 0x1f0:rt:t6:t7 0x1f4:rt:t8:t9 0x1f8:rt:t7:t8 0x200:rd:t9:t6 0x200:rs:t7:t8 0x200:rt:t8:t9 0x204:rs:t9:t6 0x204:rt:t3:t7 0x208:rt:t3:t7 0x20c:rt:t0:t1 0x210:rt:t2:a2 0x218:rt:t4:t8 0x220:rt:a3:t0 0x228:rt:t4:t8 0x248:rt:t5:t9 0x24c:rt:t2:at 0x250:rs:t5:t9 0x254:rs:t2:t6 0x254:rt:t6:at 0x284:rt:t0:t1 0x298:rt:t0:t1 0x29c:rt:t3:t6 0x2a4:rd:t4:t7 0x2a4:rt:t3:t6 0x2a8:rs:t4:t7 0x2ac:rt:t5:t8 0x2b4:rs:t5:t8 0x2b8:rd:t6:t9 0x2bc:rd:t7:t6 0x2bc:rt:t6:t9 0x2c0:rs:t7:t6 0x2c0:rt:t0:t1 0x2c4:rt:t8:t7 0x2cc:rd:t9:t8 0x2cc:rt:t8:t7 0x2d0:rs:t9:t8 0x2d4:rt:t3:t9 0x2dc:rs:t3:t9 0x2e0:rd:t4:t6 0x2e4:rd:t5:t7 0x2e4:rt:t4:t6 0x2e8:rs:t5:t7 0x2ec:rt:t6:t8 0x2f4:rs:t6:t8 0x2f8:rd:t7:t9 0x2fc:rd:t8:t6 0x2fc:rt:t7:t9 0x300:rs:t8:t6 0x314:rt:t9:t7 0x31c:rt:t9:t7 0x324:rt:t3:t8 0x328:rt:t3:t8 0x36c:rt:t4:t9 0x374:rs:t4:t9 0x374:rt:t5:t6 0x378:rt:t5:t6 */
#include "types.h"

extern u8 tt_anim_tbl[];
extern u8 tt_main50_seq_tbl[];

extern u8 *tt_boss_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_board_ptr;

extern void tt_cmd_once(s32 id);
extern void tt_seq_step(u8 *tbl, s32 idx);

void tt_serve_walk_step(void)
{
    u8 *s;
    u8 *t;
    u8 *pool;
    u8 *p;
    u8 *b;
    u8 *h;
    u8 *q;
    s32 i;
    s32 x;
    s32 y;

    s = tt_boss_wk;
    t = tt_board_ptr;
    pool = tt_anim_buf;
    switch ((u32)*(u16 *)(s + 6)) {
    case 0:
        s[835] = 1;
        i = 2;
        p = s + 40;
        do {
            p[64] = 128;
            p[60] = 0;
            p[61] = 2;
            i += 1;
            p += 20;
            p[42] = (u8)*(s16 *)tt_main50_seq_tbl;
        } while (i < 4);
        p = s;
        b = s;
        h = s;
        i = 0;
        q = t + 24;
        do {
            *(s32 *)(p + 216) = (s32)tt_anim_tbl;
            p[220] = 0;
            p[221] = 2;
            p[226] = 0;
            p[232] = 0;
            b[792] = 0;
            *(s16 *)(h + 696) = 8;
            i += 4;
            *(s16 *)(h + 600) =
                (s16)(*(s16 *)(q + 30) - *(s16 *)(s + 20));
            *(s16 *)(h + 648) =
                (s16)(*(s16 *)(q + 158) - *(s16 *)(s + 22) - 8);
            p += 20;
            b += 1;
            h += 2;
            q -= 4;
            p[204] = 128;
        } while (i < 28);
        i = 1;
        *(s32 *)(s + 376) = (s32)tt_anim_tbl;
        s[380] = 0;
        s[381] = 2;
        s[386] = 0;
        s[392] = 0;
        s[800] = 0;
        *(s16 *)(s + 712) = 8;
        x = *(s16 *)(t + 3890);
        y = *(s16 *)(s + 20);
        *(s16 *)(s + 616) = (s16)(x - y);
        x = *(s16 *)(t + 4018);
        y = *(s16 *)(s + 22);
        s[384] = 128;
        *(s16 *)(s + 664) = (s16)(x - y - 8);
        p = s + 20;
        b = s + 1;
        h = s + 2;
        q = t + 3856;
        do {
            *(s32 *)(p + 376) = (s32)tt_anim_tbl;
            p[380] = 0;
            p[381] = 2;
            p[386] = 0;
            p[392] = 0;
            b[800] = 0;
            *(s16 *)(h + 712) = 8;
            *(s16 *)(h + 616) =
                (s16)(*(s16 *)(q + 30) - *(s16 *)(s + 20));
            *(s16 *)(h + 664) =
                (s16)(*(s16 *)(q + 158) - *(s16 *)(s + 22) - 8);
            p[384] = 128;
            i += 1;
            p += 20;
            b += 1;
            h += 2;
            q -= 4;
        } while (i != 7);
        *(s16 *)(s + 876) = 0;
        *(s16 *)(s + 980) = 8;
        *(s16 *)(s + 6) = 64;
        break;
    case 64:
        tt_seq_step(tt_main50_seq_tbl, 2);
        tt_seq_step(tt_main50_seq_tbl, 3);
        if ((*(u16 *)(s + 876) & 7) == 7) {
            if ((u32)*(u16 *)(s + 980) < 15U) {
                tt_cmd_once(1290);
                s[*(u16 *)(s + 980) * 20 + 64] = 128;
                s[*(u16 *)(s + 980) + 784] = 1;
                s[*(u16 *)(s + 980) * 20 + 224] = 128;
                s[*(u16 *)(s + 980) + 792] = 1;
                pool[*(u16 *)(s + 980) * 100 + 3400] = 0;
                pool[*(u16 *)(s + 980) * 100 + 2600] = 0;
            }
            if ((u32)*(u16 *)(s + 980) >= 19U) {
                *(s16 *)(s + 4) = 81;
                *(s16 *)(s + 6) = 0;
            }
            *(s16 *)(s + 980) = (s16)(*(u16 *)(s + 980) + 1);
        }
        for (i = 8; i < 15; i++) {
            tt_seq_step(tt_main50_seq_tbl, i);
        }
        for (i = 16; i != 23; i++) {
            tt_seq_step(tt_main50_seq_tbl, i);
        }
        break;
    }
    *(s16 *)(s + 876) = (s16)(*(u16 *)(s + 876) + 1);
}
