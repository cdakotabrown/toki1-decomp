/* @RodataBase: 0x8014D730 */
/* @SymbolSize: 676 */
/* @PostProc: patch_insn_reg_field 0xb4:rt:v0:a0 0x58:rt:t8:t0 0xb8:rt:v0:a0 0xc8:rt:v0:a0 0x1e4:rt:v0:t5 0x1e8:rt:t7:t9 0x1ec:rs:v0:t5 0x1ec:rt:v0:t6 0x1f0:rs:v0:t6 0x1f0:rt:t5:t7 0x1f4:rs:t5:t7 0x1f8:rt:v0:t6 0x1fc:rt:t6:t8 0x200:rt:t6:t8 0x214:rt:t7:t9 0x234:rs:t8:t0 0x234:rt:t8:t0 0x238:rt:t0:t2 0x23c:rs:t8:t0 0x23c:rt:t9:t1 0x244:rs:t9:t1 0x24c:rs:t0:t2 0x24c:rt:t0:t2 0x254:rs:t0:t2 0x254:rt:t1:t3 0x25c:rs:t1:t3 */
/* tt_widget_step - TT overlay, Address: 0x801449F4, Size: 676 bytes (169 insns) */
/* 7-way jr table is real rodata: `@RodataBase: 0x8014D730` */
#include "types.h"

extern void tt_cmd_once();
extern void tt_obj_kind_set();
extern void trail_init();
extern void tt_widget_pair_init(void);

extern s32 tt_scroll_wk;
extern s32 tt_stage_wk;
extern s32 tt_player_wk;
extern s32 tt_trail_buf;

void tt_widget_step(u8 *p)
{
    u8 *rec;
    u8 *q;
    u8 *pa;
    u8 *pb;
    u8 *it;
    s32 step;
    s32 i;
    s32 tmp;

    rec = (u8 *)tt_player_wk;
    step = *(u16 *)(p + 4);
    switch (step) {
    case 32:
        break;
    case 64:
    case 96:
        switch (p[88]) {
        case 0:
            tt_cmd_once(1546);
            p[0] = 0;
            return;
        case 1:
            tt_cmd_once(1541);
            tt_obj_kind_set(p, 40);
            rec[85] = 9;
            ((u8 *)tt_trail_buf)[146] = 9;
            *(s32 *)(p + 52) = 0;
            *(s32 *)(p + 44) = 0;
            *(s32 *)(p + 40) = 0;
            *(s16 *)(p + 98) = 0;
            *(s16 *)(p + 4) = 144;
            p[13] = 0;
            p[14] = 0;
            p[15] = 0;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 10) = 0;
            return;
        case 2:
            tt_cmd_once(1538);
            tt_obj_kind_set(p, 41);
            rec[84] = rec[84] + 1;
            *(s32 *)(p + 52) = 0;
            *(s32 *)(p + 44) = 0;
            *(s32 *)(p + 40) = 0;
            *(s16 *)(p + 98) = 0;
            *(s16 *)(p + 4) = 144;
            p[13] = 0;
            p[14] = 0;
            p[15] = 0;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 10) = 0;
            return;
        case 3:
            tt_cmd_once(1540);
            if (rec[83] == 0) {
                tt_obj_kind_set(p, 42);
                rec[83] = 1;
                it = (u8 *)tt_trail_buf;
                for (i = 0; i != 3; i++) {
                    trail_init(it);
                    it += 164;
                }
            }
            *(s32 *)(p + 52) = 0;
            *(s32 *)(p + 44) = 0;
            *(s32 *)(p + 40) = 0;
            *(s16 *)(p + 98) = 0;
            *(s16 *)(p + 4) = 144;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
            return;
        case 6:
            tt_cmd_once(1539);
            tt_obj_kind_set(p, 43);
            tmp = rec[84] - 1;
            rec[84] = (u8)tmp;
            if ((u8)tmp == 0) {
                rec[84] = 1;
            }
            *(s32 *)(p + 52) = 0;
            *(s32 *)(p + 44) = 0;
            *(s32 *)(p + 40) = 0;
            *(s16 *)(p + 98) = 0;
            *(s16 *)(p + 4) = 144;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
            return;
        }
        break;
    case 112:
        q = (u8 *)tt_player_wk;
        if (q[82] != 0) {
            return;
        }
        pb = (u8 *)tt_stage_wk;
        pa = (u8 *)tt_scroll_wk;
        if ((u32)*(u16 *)(pb + 2) < 1025U) {
            *(s32 *)(pa + 4) = 0x8000;
            *(s32 *)(pa + 0) = 0x8000;
        }
        tt_widget_pair_init();
        *(s16 *)(rec + 4) = 96;
        *(s16 *)(rec + 10) = 0;
        *(s16 *)(rec + 8) = 0;
        *(s16 *)(rec + 6) = 0;
        rec[15] = 0;
        rec[14] = 0;
        rec[13] = 0;
        p[0] = 0;
        break;
    }
}
