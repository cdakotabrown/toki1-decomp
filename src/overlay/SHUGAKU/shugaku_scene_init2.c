/* @SymbolSize: 272 */
/* shugaku_scene_init2 - SHUGAKU overlay, Address: 0x80137FE4, Size: 280 bytes (70 insns) */
#include "types.h"
extern u8 ba_string;
extern u8 shugaku_scene_init2_str0;

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void strcpy(s32 a0, s32 a1);
extern void k_reset(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void dec_bg_cache_step(s32 a0);
extern void sub_sub_step_up(void);

extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;
extern u8 bg_change_tbl2;
extern u8 shugaku_place;

void shugaku_scene_init2(void)
{
    s32 v0;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    shared_system_state_reset();
    strcpy((s32)&ba_string, (s32)&shugaku_scene_init2_str0);
    k_reset(1);
    vblank_flag = 0;
    vb_ready = 1;
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    switch ((s32)shugaku_place) {
        case 0:
            dec_bg_cache_step(16924);
            break;
        case 1:
            dec_bg_cache_step(16906);
            break;
        default:
            dec_bg_cache_step(16915);
            break;
    }
    sub_sub_step_up();
}
