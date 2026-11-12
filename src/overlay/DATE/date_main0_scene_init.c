/* @PostProc: scheduler_normalizer */
/* Address: 0x801446A0, Size: 428 bytes (107 insns) */
#include "types.h"
#include "tokimeki/sprite.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void shared_system_state_reset(void);
extern void text_sys_init(void);
extern void k_reset(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void date_name_set(s32 a0);
extern void yazirushi_off(void);
extern void sub_sub_step_up(void);

extern u8 screen_bri;
extern u8 fade_bri;
extern u8 bg_change_tbl2;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 day_flag;
extern u8 machiawase_mode;
extern volatile u8 girl_no;
extern s32 frame_counter;
extern u8 now_basyo;
extern s32 event_girl_no;
extern s16 now_serifu;
extern u8 date_machi_mode;
extern u8 date_machi_done;
extern s16 date_now_serifu0;
extern s16 date_now_serifu0_in;
extern u8 date_event_flag;
extern u8 now_girl;
extern u8 bustup2_cw_bright;
extern s32 bustup2_cw_attr;
extern s32 bustup_cw_attr;
extern s32 Sprites;
extern u8 date_light_on;
extern s32 rpg_bat_arg;

void date_main0_scene_init(void)
{
    s32 cur;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    vblank_flag = 0;
    vb_ready = 1;
    day_flag = day_flag | 0x08;
    text_sys_init();
    k_reset(1);
    hizuke_init();
    message_window_init();
    now_basyo = machiawase_mode;
    cur = girl_no;
    event_girl_no = cur;
    now_serifu = 0;
    date_machi_mode = 1;
    if ((frame_counter & 3) == 0 && cur != 2) {
        date_machi_mode = 0;
    }
    date_machi_done = 0;
    date_now_serifu0 = 0;
    date_now_serifu0_in = 0;
    date_event_flag = 0;
    now_girl = cur;
    date_name_set(cur);
    bustup2_cw_bright = 0;
    bustup2_cw_attr = bustup2_cw_attr | 0x81000000;
    bustup_cw_attr = bustup_cw_attr | 0x81000000;
    Sprites = Sprites | 0x81000000;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[1].attribute) = (*(s32 *)&((SPRITE_REC *)&Sprites)[1].attribute) | 0x81000000;
    yazirushi_off();
    date_light_on = 1;
    rpg_bat_arg = 0;
    sub_sub_step_up();
}
