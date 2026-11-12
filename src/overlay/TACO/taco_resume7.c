/* @SymbolSize: 236 */
/* Address: 0x80142CB0 */
#include "types.h"

extern u8 anim_fade;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern s32 class_max;
extern s32 taco_rec;

extern void goto_step(s32);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void draw2d3d(s32, s32);
extern void tpage_buf_clear(void);
extern void class_switch(s32);
extern void class_clear_all(s32);
extern void text_sys_init(void);
extern void dec_bg_reset(void);
extern void hizuke_disp_switch(s32);
extern void hizuke_init(void);
extern void message_disp_switch(s32);
extern void message_window_init(void);
extern void cursor_init(s32);
extern void yazirushi_disp_switch(s32);

void taco_resume7(void)
{
    s32 val;

    text_sys_init();
    dec_bg_reset();
    draw2d3d(1, 0);
    class_switch(1);
    val = taco_rec;
    class_max = 159;
    if (((u32)(val << 30) >> 31) == 1) {
        class_clear_all(0);
        tpage_buf_clear();
        anim_fade = 254;
        goto_step(ret_step);
        goto_sub_step(ret_sub_step);
        goto_sub_sub_step(ret_sub_sub_step);
        cursor_init(0);
        yazirushi_disp_switch(0);
        message_window_init();
        hizuke_init();
        message_disp_switch(1);
        hizuke_disp_switch(1);
    } else {
        goto_step(18);
    }
}
