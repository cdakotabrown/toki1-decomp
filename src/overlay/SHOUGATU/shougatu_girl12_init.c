/* @CompileEnv: IDO52_CONST_INREG_VALUES=12 */
/* Address: 0x8013B858 */
/* Size: 372 bytes (93 instructions) */
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
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern s32 strcpy(u8 *, u8 *);
extern void shougatu_ptrs1_set(void);

extern u8 bg_change_tbl2;
extern u8 ba_string[];
extern u8 screen_w[];
extern u8 screen_bri;
extern s8 now_girl;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 fade_bri;
extern u8 shougatu_init2_str0[];
extern s32 shougatu_appear_ss_serifu;
extern s32 shougatu_appear_ss_serifu_kind;
extern s32 shougatu_appear_sss_sound;
extern u16 shougatu_appear_now_serifu;
extern u16 shougatu_appear_now_serifu_in;
extern u8 shougatu_appear_any;
extern s32 shougatu_appear_serifu_tbl[];
extern s32 shougatu_appear_serifu_kind_tbl[];
extern s32 shougatu_appear_sound_tbl[];

void shougatu_girl12_init(void)
{
    s16 i;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(ba_string, shougatu_init2_str0);
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
    now_girl = 12;
    date_name_set(now_girl);
    shougatu_ptrs1_set();
    shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[3];
    shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[3];
    shougatu_appear_sss_sound = shougatu_appear_sound_tbl[3];
    shougatu_appear_now_serifu = 0;
    shougatu_appear_now_serifu_in = 0;
    shougatu_appear_any = 0;
    for (i = 0; i < 11; i++) {
        if (screen_w[i * 56 + 457] & 1) {
            shougatu_appear_any = 1;
        }
    }
    sub_sub_step_up();
}
