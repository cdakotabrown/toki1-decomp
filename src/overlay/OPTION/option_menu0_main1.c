/* @SymbolSize: 1092 */
/* @RodataBase: 0x8013D0D4 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @PostProc: patch_insn_reg_field 0xb4:rt:a0:v0 0xb8:rs:a0:v0 0x2b8:rt:t5:t6 0x2bc:rs:t5:t6 0x2bc:rt:t5:t6 0x2c0:rt:t4:t5 0x2c4:rs:t5:t6 0x2c4:rt:t6:t7 0x2c8:rd:t7:t8 0x2c8:rs:t4:t5 0x2c8:rt:t6:t7 0x2d4:rt:t7:t8 0x2e0:rd:t8:t9 0x2e4:rt:t8:t9 0x2f0:rt:t2:t4 0x2f4:rd:t0:t2 0x2f8:rs:t0:t2 0x2f8:rt:t1:t3 0x2fc:rs:t2:t4 0x2fc:rt:t3:t6 0x300:rs:t1:t3 0x300:rt:t3:t6 0x314:rd:t4:t7 0x318:rt:t4:t7 0x324:rt:t9:t2 0x328:rd:t7:t0 0x32c:rs:t7:t0 0x32c:rt:t8:t1 0x330:rs:t9:t2 0x330:rt:t0:t4 0x334:rd:t2:t3 0x334:rs:t8:t1 0x334:rt:t0:t4 0x33c:rt:t2:t3 0x348:rd:t1:t6 0x34c:rs:t1:t6 0x358:rt:t5:t9 0x35c:rs:t5:t9 0x35c:rt:t5:t9 0x360:rd:t3:t8 0x364:rs:t5:t9 0x364:rt:t4:t0 0x368:rd:t6:t2 0x368:rs:t3:t8 0x368:rt:t4:t0 0x374:rt:t6:t2 0x380:rt:t7:t1 0x384:rs:t7:t1 0x390:rt:t8:t6 0x394:rs:t8:t6 0x394:rt:t8:t6 0x398:rt:t9:t3 0x39c:rs:t8:t6 0x39c:rt:t0:t5 0x3a0:rd:t2:t7 0x3a0:rs:t9:t3 0x3a0:rt:t0:t5 0x3ac:rt:t2:t7 0x3b8:rd:t1:t9 0x3bc:rt:t1:t9 0x3c8:rt:t6:t4 0x3cc:rd:t3:t2 0x3d0:rs:t3:t2 0x3d0:rt:t4:t1 0x3d4:rs:t6:t4 0x3d4:rt:t7:t6 0x3d8:rd:t8:t3 0x3d8:rs:t4:t1 0x3d8:rt:t7:t6 0x3e0:rt:t8:t3 0x3ec:rd:t9:t5 0x3f0:rt:t9:t5 0x3fc:rt:t5:t2 0x400:rd:t2:t8 0x404:rs:t2:t8 0x404:rt:t1:t0 0x408:rs:t5:t2 0x408:rt:t3:t4 0x40c:rd:t6:t1 0x40c:rs:t1:t0 0x40c:rt:t3:t4 0x410:rt:t6:t1 0x414:rt:t4:t6 0x418:rs:t4:t6 0x418:rt:t4:t6 0x420:rs:t4:t6 0x420:rt:t7:t3 0x424:rs:t7:t3 */
/* Address: 0x8013A80C, Size: 1092 bytes (273 instructions) */
#include "types.h"

typedef union {
    u32 w;
    u16 h;
    struct {
        u8 b0;
        u8 b1;
        u8 b2;
        u8 b3;
    } b;
} OptWord;

typedef struct {
    u8      pad0[244];
    union {
        u16 h[2];
        u8  b[4];
    } f244;
    OptWord f248;
    u8      pad1[208];
    OptWord f460;
} OptCfg;

extern OptCfg screen_w;
extern u8  season_no;
extern u8  now_year;
extern u8  now_month;
extern u8  now_day;
extern u8  now_youbi;
extern u16 pass_days;
extern u8  byouki_kind;
extern u8  p_birthday;
extern u8  girl0_birth;
extern u32 pad_data;
extern u32 pad_trig;
extern u8  now_icon;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 option_menu0_lbl_tbl[];

extern void menu_check();
extern void menu_cursor_move();
extern void menu_bar_show();
extern void debug_string_show();
extern void debug_number_show();
extern void hizuke_advance();
extern void goto_sub_sub_step();
extern void _game_work_init_set();
extern s32  option_value_step();

void option_menu0_main1(void)
{
    OptCfg *p;
    s32 *q;
    s32 i;
    s32 j;
    s32 c;
    s32 v;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(0);
    menu_bar_show(0);

    p = &screen_w;
    q = option_menu0_lbl_tbl;
    for (i = -96; i < 96; i += 12) {
        debug_string_show(*q, -32, i);
        q++;
    }

    debug_number_show(now_year, 32, -96);
    debug_number_show(now_month, 32, -84);
    debug_number_show(now_day, 32, -72);
    debug_number_show(now_youbi, 32, -60);
    debug_number_show(season_no, 32, -48);
    debug_number_show(pass_days, 32, -36);

    debug_number_show(p->f248.w & 0xF, 32, 12);
    debug_number_show((p->f248.w << 23) >> 27, 32, 24);
    debug_number_show((p->f248.w << 21) >> 30, 32, 36);
    debug_number_show((u32)p->f244.h[0] >> 12, 32, 48);
    debug_number_show(p->f460.w & 0xF, 32, 60);
    debug_number_show((p->f460.w << 23) >> 27, 32, 72);
    debug_number_show((p->f460.w << 21) >> 30, 32, 84);

    if ((pad_data & 0x90) != 0) {
        switch (now_icon) {
        case 0:
        case 1:
            break;
        case 2:
            if ((pad_trig & 0x90) != 0) {
                c = now_youbi;
                q = (s32 *)(u32)now_month;
                if (c != 6) {
                    for (j = c; j < 6; j++) {
                        hizuke_advance();
                    }
                } else {
                    j = 0;
                    i = 7;
                    while (j != i) {
                        hizuke_advance();
                        j++;
                    }
                }
                if ((u32)q != now_month) {
                    goto_sub_sub_step(254);
                }
            }
            break;
        case 4:
            season_no = option_value_step(season_no, 3, 0);
            break;
        case 9:
            v = option_value_step(p->f248.w & 0xF, 12, 1);
            *(volatile u8 *)&p->f248 = (v & 0xF) | (p_birthday & 0xFFF0);
            break;
        case 10:
            v = option_value_step((p->f248.w << 23) >> 27, 31, 1);
            p->f248.h = ((v << 4) & 0x1F0) | (p->f248.h & 0xFE0F);
            break;
        case 11:
            v = option_value_step((p->f248.w << 21) >> 30, 3, 0);
            p->f248.b.b1 = ((v << 1) & 6) | (p->f248.b.b1 & 0xFFF9);
            break;
        case 12:
            v = option_value_step((u32)p->f244.h[0] >> 12, 11, 0);
            *(volatile u8 *)&p->f244.b[1] = (v << 4) | (byouki_kind & 0xFF0F);
            break;
        case 13:
            v = option_value_step(p->f460.w & 0xF, 12, 1);
            *(volatile u8 *)&p->f460 = (v & 0xF) | (girl0_birth & 0xFFF0);
            break;
        case 14:
            v = option_value_step((p->f460.w << 23) >> 27, 31, 1);
            p->f460.h = ((v << 4) & 0x1F0) | (p->f460.h & 0xFE0F);
            break;
        case 15:
            v = option_value_step((p->f460.w << 21) >> 30, 3, 0);
            p->f460.b.b1 = ((v << 1) & 6) | (p->f460.b.b1 & 0xFFF9);
            break;
        }
    }

    if ((pad_trig & 0x800) != 0) {
        _game_work_init_set();
        goto_sub_sub_step(0);
    }
}
