/* Function: title_white_out */
/* Address: 0x800621D8 */
/* Size: 344 bytes (86 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u32 step_counter;
extern s32 pad_trig;
extern s32 Bg;
extern u8 back_clear_g;
extern u8 back_clear_r;
extern u8 back_clear_b;
extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern s32 cal_cw_attr;
extern u8 cal_cw_disp;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern s16 cal_cw_x;
extern s16 cal_cw_y;

extern void white_light(u32);
extern void all_object_disp_off(void);
extern void cursor_init(s32);
extern void set_flag_b3_high_bit(s32);
extern void back_clear_switch(s32);
extern void class_clear_n(s32);
extern void sub_step_up(void);
extern void sub_sub_step_up(void);

void title_white_out(void) {
    u32 val;
    u8 *p797;
    u8 *p796;
    u8 *p798;

    val = step_counter;
    white_light(val >> 1);

    if (step_counter >> 1 >= 255 || (pad_trig & 0x860) != 0) {
        Bg = 0x80000000;
        all_object_disp_off();
        cursor_init(0);
        set_flag_b3_high_bit(0);
        back_clear_switch(1);
        p797 = &back_clear_g;
        p796 = &back_clear_r;
        p798 = &back_clear_b;
        *p796 = *p797 = *p798 = 255;
        class_clear_n(96);

        cal_cw_prio = 13;
        cal_cw_mode = 64;
        cal_cw_attr = 0x01000000;
        cal_cw_disp = 164;
        cal_cw_img = 0x801EAB3C;
        cal_cw_anim_tbl = 0x801EAB6C;
        cal_cw_hdr = 0x801EAB20;
        cal_cw_flag4 = 8;
        cal_cw_sel = 128;
        cal_cw_x = -160;
        cal_cw_y = -120;

        if ((pad_trig & 0x860) != 0) {
            sub_step_up();
        } else {
            sub_sub_step_up();
        }
    }
}
