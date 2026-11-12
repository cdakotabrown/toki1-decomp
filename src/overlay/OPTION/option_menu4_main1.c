/* @RodataBase: 0x8013D194 */
/* @PostProc: patch_insn_reg_field 0x22c:rt:t5:t6 0x230:rs:t5:t6 0x230:rt:t5:t6 0x234:rt:t4:t5 0x238:rs:t5:t6 0x238:rt:t6:t7 0x23c:rd:t7:t8 0x23c:rs:t4:t5 0x23c:rt:t6:t7 0x248:rt:t7:t8 0x254:rd:t8:t9 0x258:rt:t8:t9 0x264:rt:t2:t4 0x268:rs:t2:t4 0x268:rt:t2:t4 0x26c:rd:t0:t2 0x270:rs:t0:t2 0x270:rt:t1:t3 0x274:rs:t2:t4 0x274:rt:t3:t6 0x278:rs:t1:t3 0x278:rt:t3:t6 0x290:rt:t4:t7 0x294:rs:t4:t7 0x2a0:rt:t6:t0 0x2a4:rt:t8:t9 0x2a8:rs:t6:t0 0x2a8:rt:t7:t1 0x2ac:rd:t9:t2 0x2ac:rs:t7:t9 0x2ac:rt:t8:t1 0x2b4:rt:t9:t2 */
/* Address: 0x8013C018, Size: 952 bytes (238 instructions) */
#include "types.h"

extern void menu_check();
extern void menu_cursor_move();
extern void menu_bar_show();
extern void debug_string_show();
extern void debug_number_show();
extern u32 option_value_step();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 option_menu4_lbl_tbl[];
extern u8 screen_w[];
extern u8 schedule_kind;
extern u8 schedule_arg;
extern u8 reserved_date_b3;
extern u8 machiawase_mode;
extern u8 machiawase_basyo;
extern u8 machiawase_basyo2;
extern u8 machiawase_basyo3;
extern u8 now_girl;
extern u8 girl_no;
extern u8 now_icon;
extern s32 pad_data;
extern s32 event_girl_no;
extern s32 work_tarao;

void option_menu4_main1(void)
{
    s32 *rows;
    s32 y;
    u32 v;
    u8 *cfg;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(0);
    menu_bar_show(0);

    rows = option_menu4_lbl_tbl;
    y = -96;
    do {
        debug_string_show(*rows, -32, y);
        y += 12;
        rows++;
    } while (y != 96);

    cfg = screen_w;
    debug_number_show(schedule_kind, 32, -96);
    debug_number_show(schedule_arg, 32, -84);
    debug_number_show(cfg[1359] & 1, 32, -48);
    debug_number_show((u32)(*(s32 *)&cfg[1356] << 3) >> 28, 32, -36);
    debug_number_show(*(s32 *)&cfg[1356] & 0x1F, 32, -24);
    debug_number_show(machiawase_mode, 32, 0);
    debug_number_show(machiawase_basyo, 32, 12);
    debug_number_show(machiawase_basyo2, 32, 24);
    debug_number_show(machiawase_basyo3, 32, 36);
    debug_number_show(now_girl, 32, 48);
    debug_number_show(girl_no, 32, 60);
    debug_number_show(event_girl_no, 32, 72);
    debug_number_show(work_tarao, 32, 84);

    if ((pad_data & 0x90) != 0) {
        switch (now_icon) {
        case 0:
            schedule_kind = option_value_step(schedule_kind, 10, 0);
            break;
        case 1:
            schedule_arg = option_value_step(schedule_arg, 36, 0);
            break;
        case 4:
            v = option_value_step(reserved_date_b3 & 1, 1, 0);
            reserved_date_b3 = (v & 1) | (reserved_date_b3 & 0xFFFE);
            break;
        case 5:
            v = option_value_step(
                (u32)(*(s32 *)&cfg[1356] << 3) >> 28, 10, 0);
            *(volatile u8 *)&cfg[1359] =
                ((v << 1) & 0x1E) | (reserved_date_b3 & 0xFFE1);
            break;
        case 6:
            v = option_value_step(*(s32 *)&cfg[1356] & 0x1F, 21, 0);
            cfg[1356] = (v & 0x1F) | (cfg[1356] & 0xFFE0);
            break;
        case 8:
            machiawase_mode = option_value_step(machiawase_mode, 21, 0);
            break;
        case 9:
            machiawase_basyo = option_value_step(machiawase_basyo, 21, 0);
            break;
        case 10:
            machiawase_basyo2 = option_value_step(machiawase_basyo2, 21, 0);
            break;
        case 11:
            machiawase_basyo3 = option_value_step(machiawase_basyo3, 21, 0);
            break;
        case 12:
            now_girl = option_value_step(now_girl, 10, 0);
            break;
        case 13:
            girl_no = option_value_step(girl_no, 10, 0);
            break;
        case 14:
            event_girl_no = option_value_step(event_girl_no, 12, 0);
            break;
        case 15:
            work_tarao = option_value_step(work_tarao, 8, 1);
            break;
        }
    }
}
