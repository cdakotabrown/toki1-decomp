/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: remap_stack_slots --map '176:164,160:148,144:132,128:116' */
/* @PostProc: patch_insn_reg_field 0x9c:rt:s0:s2 0xa0:rs:s0:s2 0xa0:rt:s0:s2 0xa4:rt:s1:s0 0xa8:rt:s2:s1 0xc4:rs:s1:s0 0xc8:rs:s2:s1 0xd4:rs:s2:s1 0xd8:rs:s1:s0 0xdc:rs:s0:s2 0xf4:rs:s0:s2 0xf4:rt:s0:s2 0xf8:rs:s1:s0 0xf8:rt:s1:s0 0xfc:rs:s2:s1 0xfc:rt:s2:s1 0x108:rs:s0:s2 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=160 */
#include "types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 olh_init_str0[];
extern u8 olh_init_str1[];
extern u8 olh_init_str2[];
extern s32 olh_top_tbl[];
extern s32 olh_top_tbl_end[];

extern void text_sys_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void basyo_init(void *a0);
extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *a4,
                          s16 *a5);
extern void k_disp_start(s32 a0);
extern void sub_step_up(void);

void olh_init(void)
{
    s16 x_buf[8];
    s16 y_buf[8];
    s16 w_buf[8];
    s16 h_buf[8];
    s16 *s0_x;
    s16 *s1_y;
    s16 *s4_w;
    s16 *s5_h;
    s32 *s2;
    s32 s3_y;
    s32 fp_h;
    s32 s7_w;
    s32 s6_x;
    s32 s6_ret;

    text_sys_init();
    class_clear_all(0);
    back_clear_switch(1);

    back_clear_g = 160;
    back_clear_r = 160;
    back_clear_b = 160;

    basyo_init(olh_init_str0);
    set_kanji_string(-104, 92, 2, (s32)olh_init_str1, 0);
    k_sub_reset_point_set();

    s2 = olh_top_tbl;
    s0_x = x_buf;
    s1_y = y_buf;
    s3_y = -60;
    s4_w = w_buf;
    s5_h = h_buf;
    fp_h = 16;
    s7_w = 128;
    s6_x = -96;

    do {
        *s0_x = (s16)s6_x;
        *s1_y = (s16)s3_y;
        *s4_w = (s16)s7_w;
        *s5_h = (s16)fp_h;
        s6_ret = set_kanji_string(*s0_x, *s1_y, 15, *s2, 0);
        s2++;
        s0_x++;
        s1_y++;
        s3_y += 16;
        s4_w++;
        s5_h++;
    } while (s2 != olh_top_tbl_end);

    menu_set(0, 8, x_buf, y_buf, w_buf, h_buf);

    s6_ret = set_kanji_string(-16, -80, 15, (s32)olh_init_str2, 0);
    k_disp_start(s6_ret);
    sub_step_up();
}
