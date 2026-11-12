/* omimai_scene_init - OMIMAI overlay, Address: 0x80132670, Size: 280 bytes (70 insns) */
#include "types.h"

extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;
extern u8 bg_change_tbl2;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern u8 girl_no;
extern u16 basyo_pack;
extern u8 girl4_basyo;
extern u8 now_girl;

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void class_clear_all(s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern s32 ba_string;
extern void strcpy(s32 *, u8 *);
extern u8 omimai_scene_init_str0[];
extern void shared_system_state_reset(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32);
extern void omimai_serifu_set(void);
extern void sub_sub_step_up(void);

void omimai_scene_init(void)
{
    u32 v0;
    u16 t8;
    u8 t0;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(&ba_string, omimai_scene_init_str0);
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    now_serifu = 0;
    v0 = girl_no;
    now_serifu_in = 0;
    if (v0 == 4) {
        t8 = basyo_pack;
        t0 = girl4_basyo;
        if (((u32)t0 >> 4) == ((u32)t8 >> 12)) {
            now_serifu = 1;
        }
    }
    now_girl = girl_no & 0xFF;
    date_name_set(girl_no & 0xFF);
    omimai_serifu_set();
    sub_sub_step_up();
}
