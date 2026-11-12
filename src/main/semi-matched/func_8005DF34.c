/* Function: func_8005DF34 */
/* Address: 0x8005DF34 */
/* Size: 268 bytes (67 instructions) */
/* Status: ASM_FINAL (hand-edited assembly — will not match from C) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Status: CONFIRMED UNMATCHABLE FROM C */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void system_config_disp(s32);
extern void k_sub_reset_point_set(void);
extern void set_kanji_string(s32, s32, s32, void *, s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern u8 mascot_flag;
extern u8 mascot_flag2;
extern s8 now_icon;
extern s8 system_config_menu_init_str1;

static s32 func_8005DFF4_helper(void) {
    s32 count;

    count = 0;
    if (mascot_flag != 0) {
        count = 1;
    }
    if (mascot_flag2 != 0) {
        count++;
    }

    return count;
}

void func_8005DF34(void) {
    s16 buf[32];
    s16 *p1;
    s16 *p2;
    s32 val;
    s32 masked;

    system_config_disp(-1);

    if (func_8005DFF4_helper() == 0) {
        buf[14] = 0;
    }

    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &system_config_menu_init_str1, 0);

    p1 = &buf[8];
    p2 = &buf[0];
    menu_set(0, 8, &buf[24], &buf[16], p1, p2);

    val = now_icon;
    masked = val & 7;
    if (val < 0 && masked != 0) {
        masked -= 8;
    }
    now_icon = (s8)masked;

    k_sub_disp_start(0);
    sub_sub_step_up();
}
