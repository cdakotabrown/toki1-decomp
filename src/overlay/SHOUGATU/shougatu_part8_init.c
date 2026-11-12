/* @PostProc: patch_insn_reg_field 0xa4:rt:v0:a0 0xac:rt:v0:a0 0xb8:rt:v0:a0 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
/* @SymbolSize: 284 */
/* shougatu_part8_init - SHOUGATU overlay, Address: 0x80137E54, Size: 284 bytes. */
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
extern void date_name_set(void);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern s32 strcpy(void *, void *);
extern void shougatu_ptab_init(void);

extern u8 bg_change_tbl2;
extern u8 g_name;
extern u8 screen_bri;
extern u8 girl_no;
extern u8 now_girl;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 fade_bri;
extern u8 shougatu_part8_init_str0;
extern s32 shougatu_date_serifu_tbl;
extern s32 shougatu_date_serifu_kind_tbl;
extern s32 shougatu_date_sound_tbl;
extern s32 shougatu_date_ss_serifu;
extern s32 shougatu_date_ss_serifu_kind;
extern s32 shougatu_date_sss_sound;
extern u8 shougatu_date_kind;

void shougatu_part8_init(void)
{
    s32 eight;

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
    eight = 8;
    girl_no = (u8)eight;
    now_girl = (u8)eight;
    date_name_set();
    if ((u32)shougatu_date_kind < 3U) {
        strcpy(&g_name, &shougatu_part8_init_str0);
    }
    shougatu_ptab_init();
    shougatu_date_ss_serifu = shougatu_date_serifu_tbl;
    shougatu_date_ss_serifu_kind = shougatu_date_serifu_kind_tbl;
    shougatu_date_sss_sound = shougatu_date_sound_tbl;
    sub_sub_step_up();
}
