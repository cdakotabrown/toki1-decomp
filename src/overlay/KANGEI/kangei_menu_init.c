/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void cursor_init(s32);
extern void set_flag_b3_high_bit(s32);
extern void icon_init(void);
extern void icon_can_use_set(s32, s32);
extern void icon_cursor_move(s32);
extern void parameter_show_init(void);
extern void k_reset(s32);
extern void kangei_bank_set2(void);
extern void kangei_msg_reset(void);
extern void date_name_set(s32);
extern void sub_sub_step_up(void);

extern u8 kangei_birth_flag;
extern u8 cal_cw_disp;
extern u8 fade_bri;
extern u8 cw97_disp;
extern u8 now_girl;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;
extern u8 kangei_icon_flag;

void
kangei_menu_init(void)
{
    s16 i;
    u8 v1, v2;
    u8 sa;

    kangei_birth_flag = 0;
    cursor_init(0);
    set_flag_b3_high_bit(0);
    icon_init();
    i = 0;
    do {
        icon_can_use_set(i, 0);
        i++;
    } while (i < 15);
    icon_cursor_move(1);
    parameter_show_init();
    parameter_show_init();
    v1 = cal_cw_disp;
    fade_bri = 128;
    cal_cw_disp = (u8)(v1 & 0x7F);
    v2 = cw97_disp;
    cw97_disp = (u8)(v2 & 0x7F);
    k_reset(1);
    now_girl = 0;
    kangei_now_s = 0;
    kangei_now_s_in = 0;
    kangei_bank_set2();
    kangei_msg_reset();
    sa = now_girl;
    date_name_set(sa);
    kangei_icon_flag = 1;
    sub_sub_step_up();
}
