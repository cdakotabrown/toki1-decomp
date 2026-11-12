/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1 */
/* @SymbolStart: 0x801362C8 */
/* @SymbolSize: 400 */
/* @PostProc: patch_insn_reg_field 0x7c:rt:t6:v1 0x80:rs:t6:v1 0x80:rt:t6:v1 0x88:rs:t6:v1 0x88:rt:t7:t6 0x90:rt:t7:t6 0xc8:rt:t8:t7 0xcc:rs:t8:t7 0xcc:rt:t8:t7 0xd4:rs:t8:t7 0xd4:rt:t9:t8 0xd8:rs:t9:t8 0xf0:rt:t1:t0 0xf4:rd:t0:t9 0xf8:rd:t1:t0 0xf8:rs:t1:t0 0xf8:rt:t0:t9 0xfc:rs:t1:t0 0xfc:rt:t1:t0 0x100:rt:t3:t2 0x104:rd:t2:t1 0x10c:rd:t3:t2 0x10c:rs:t3:t2 0x10c:rt:t2:t1 0x110:rs:t3:t2 0x110:rt:t3:t2 0x114:rt:t1:t0 0x11c:rt:t4:t3 0x120:rs:t4:t3 0x120:rt:t4:t3 0x124:rt:t3:t2 0x12c:rs:t4:t3 0x130:rt:t9:v1 0x134:rt:t5:t4 0x138:rs:t5:t4 0x138:rt:t5:t4 0x140:rs:t5:t4 0x144:rt:t6:t5 0x148:rs:t6:t5 0x148:rt:t6:t5 0x150:rs:t6:t5 0x150:rt:t7:t6 0x158:rt:t7:t6 0x15c:rt:t8:t7 0x160:rs:t8:t7 0x160:rt:t8:t7 0x168:rt:t8:t7 0x16c:rs:t9:v1 0x16c:rt:t9:v1 0x174:rs:t9:v1 0x174:rt:t0:t8 0x17c:rt:t0:t8 */
/* tel_cal_select30 - TEL overlay, Address: 0x801362D0, Size: 392 bytes (98 insns) */
#include "types.h"

extern u8 step_mode;
extern s32 pad_trig;
extern s8 menu_csr;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8 now_month;
extern u8 now_year;
extern s32 tel_sasoi_day;
extern s32 tel_sasoi_month;
extern s32 tel_sasoi_year;
extern s32 tel_cal_month_tbl[];
extern s32 tel_cal_day_tbl[];
extern s32 tel_cal_select30_str0;

extern void cal_class_init(void);
extern void cal_sprite_init(void);
extern s32 tel_cal_menu_set(void);
extern void text_sys_init(void);
extern s32 set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);
extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern void menu_bar_show(s32);
extern void cal_sprite_disp_switch(s32);
extern void goto_sub_sub_step(s32);
extern void cal_bio_disp(void);

void tel_cal_select30(void)
{
    switch (step_mode) {
        case 0:
            cal_class_init();
            cal_sprite_init();
            tel_cal_menu_set();
            text_sys_init();
            set_kanji_string(-128, 50, 0, (s32)&tel_cal_select30_str0, 0);
            k_disp_start(1);
            step_mode = step_mode + 1;
            break;
        case 1:
            menu_check(1, cursor_cw_x, cursor_cw_y);
            menu_cursor_move(1);
            menu_bar_show(1);
            if (menu_csr == -1) {
                break;
            }
            if ((pad_trig & 0x20) == 0) {
                break;
            }
            cal_sprite_disp_switch(0);
            tel_sasoi_month = tel_cal_month_tbl[menu_csr];
            tel_sasoi_day = *(s32 *)((u8 *)tel_cal_day_tbl + (menu_csr << 2));
            if (now_month == 12 && tel_sasoi_month == 1) {
                tel_sasoi_year = now_year + 1;
            } else {
                tel_sasoi_year = now_year;
            }
            step_mode = step_mode + 1;
            break;
        default:
            goto_sub_sub_step(48);
            break;
    }
    cal_bio_disp();
}
