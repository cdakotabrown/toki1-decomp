/* @CompileEnv: IDO52_CONST_INREG_VALUES=3 */
/* @CompileFlags: -cckr */
/* @SymbolSize: 240 */
#include "types.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void class_switch(s32);
extern void class_clear_all(s32);
extern void text_sys_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void date_name_set();
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void shougatu_ptab_init(void);
extern u8 bg_change_tbl2;
extern u8 screen_bri;
extern u8 now_girl;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 fade_bri;
extern s32 shougatu_date_ss_serifu;
extern s32 shougatu_date_ss_serifu_kind;
extern s32 shougatu_date_sss_sound;
extern s32 shougatu_date_serifu_tbl[];
extern s32 shougatu_date_serifu_kind_tbl[];
extern s32 shougatu_date_sound_tbl[];

void shougatu_girl3_init(void)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    date_name_set(now_girl = 3);
    shougatu_ptab_init();
    shougatu_date_ss_serifu = shougatu_date_serifu_tbl[2];
    shougatu_date_ss_serifu_kind = shougatu_date_serifu_kind_tbl[2];
    shougatu_date_sss_sound = shougatu_date_sound_tbl[2];
    sub_sub_step_up();
}
