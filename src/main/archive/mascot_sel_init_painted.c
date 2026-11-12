/* Function: mascot_sel_init */
/* Address: 0x800603B0 */
/* Size: 584 bytes (146 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 216 --new 232 */
/* @PostProc: custom_script custom_func_800603B0.py */
#include "types.h"

extern u8 mascot_flag[];
extern u8 mascot_flag2[];
extern s32 *date_name_set_tbl0[];
extern s8 menu_csr;
extern s16 cursor_work_copy_tbl0;

extern void text_sys_init(void);
extern void basyo_init(s32 a0);
extern void system_config_disp(s32 a0);
extern void set_kanji_string(s16 a0, s16 a1, s32 a2, s32 *a3, s32 arg4);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *arg4, s16 *arg5);
extern void k_sub_disp_start(s32 a0);
extern void sub_sub_step_up(void);

extern s32 mascot_sel_init_str0;
extern s32 mascot_sel_init_str1;
extern s32 mascot_sel_init_str2;

void mascot_sel_init(void) {
    s16 spC0[14];
    s16 spA4[14];
    s16 sp88[14];
    s16 sp6C[14];
    s32 s0;
    s16 *s1;
    s16 *s2;
    s16 *s3;
    s16 *s4;
    s16 temp;
    s32 pad1;
    s32 pad2;

    (void)&pad1;
    (void)&pad2;

    text_sys_init();
    basyo_init((s32)&mascot_sel_init_str0);
    system_config_disp(6);

    s0 = 0;
    s1 = spC0;
    s2 = spA4;
    s3 = sp88;
    s4 = sp6C;

    do {
        *s3 = 40;
        *s4 = 16;
        *s1 = (s16)((s0 % 3) * 43 - 8);
        *s2 = (s16)((s0 / 3) * 16 - 64);

        if (s0 == 13) {
            *s3 = 70;
            set_kanji_string(*s1, *s2, 5, &mascot_sel_init_str1, 0);
        } else if (mascot_flag[s0] || mascot_flag2[s0]) {
            set_kanji_string(*s1, *s2, 3, date_name_set_tbl0[s0], 0);
        } else {
            *s3 = 0;
        }

        s0++;
        s1++;
        s2++;
        s3++;
        s4++;
    } while (s0 != 14);

    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &mascot_sel_init_str2, 0);
    menu_set(1, 14, spC0, spA4, sp88, sp6C);

    temp = cursor_work_copy_tbl0;
    if (temp == 0 || temp >= 14) {
        menu_csr = 0;
    } else {
        menu_csr = (s8)temp;
    }

    k_sub_disp_start(0);
    sub_sub_step_up();
}
