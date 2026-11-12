/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x801444A0 */
/* @SymbolSize: 420 */
/* Address: 0x801444A8 (true entry 0x801444A0, 2 insns hoisted above .sym) */
#include "types.h"

extern void prim_init(void);
extern s32 sub_sub_step_up();
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void class_switch(s32);
extern void class_clear_all(s32);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void k_speed_set(s32);
extern s32 data_save_load_class_init(void);
extern void cursor_init(s32);
extern u8 *get_etc_yes_button(void);
extern u8 *get_etc_no_button(void);

extern u8 screen_bri;
extern u8 csr_speed;
extern s32 class_max;
extern u8 step_mode;
extern u8 cal_cw_disp;
extern u8 save_play_show_str0[];
extern u8 etc_yes_str[];
extern u8 etc_no_str[];

s32 save_play_show(void)
{
    s32 ret;
    u32 cnt;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        tpage_buf_clear();
        draw2d3d(1, 0);
        k_speed_set(0);
        csr_speed = 3;
        class_clear_all(0);
        class_switch(1);
        prim_init();
        class_max = 159;
        cursor_init(0);
        back_clear_switch(1);
        text_sys_init();
        set_kanji_string(-144, -64, 15, save_play_show_str0, 0);
        set_kanji_string(-24, 4, 15, etc_yes_str, 0);
        set_kanji_string(60, 4, 1, get_etc_yes_button(), 0);
        set_kanji_string(-24, 20, 15, etc_no_str, 0);
        set_kanji_string(60, 20, 2, get_etc_no_button(), 0);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        ret = data_save_load_class_init();
        cal_cw_disp = cal_cw_disp | 0x20;
        step_mode = step_mode + 1;
        screen_bri = 0;
        break;
    case 1:
        cnt = screen_bri;
        ret = cnt >= 129U;
        screen_bri = cnt + 1;
        if (ret) {
            ret = sub_sub_step_up();
        }
        break;
    }
    return ret;
}
