/* @SymbolSize: 520 */
/* @PostProc: scheduler_normalizer --block-aware --branch-fp --const-fp */
/* @PostProc: swap_instructions --offset1 0x1dc --offset2 0x1e0 */
/* @PostProc: patch_insn_reg_field 0x19c:rt:a1:v0 0x1a4:rs:a1:v0 0x1a4:rt:a1:v0 0x1b8:rs:a1:v0 0x1d4:rt:v0:t8 0x1dc:rd:t8:t9 0x1dc:rt:v0:t8 0x1e0:rd:t9:t0 0x1e0:rt:t8:t9 0x1e4:rs:t9:t0 0x1e4:rt:t0:t1 0x1e8:rs:t0:t1 */
#include "types.h"

extern u8  vblank_flag;
extern s32 vb_ready;
extern u8  screen_bri;
extern u8  fade_bri;
extern u8  bg_change_tbl2;
extern s16 now_serifu_shugaku;
extern s16 now_serifu_shugaku_in;
extern s32 ss_serifu_shugaku;
extern s32 ss_serifu_shugaku_kind;
extern s32 sss_sound_shugaku;
extern s32 rpg_bat_arg;
extern u8  date_place;
extern u8  girl_appear[];
extern u8  screen_w[];
extern u8  basho_num;
extern u8  ba_string[];
extern u8  shugaku_scene_init_str0[];
extern u8  g_name[];
extern u8  p_sei[];
extern s32 shugaku_serifu_tbl[];
extern s32 shugaku_serifu_kind_tbl[];
extern s32 shugaku_sound_tbl[];

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void strcpy(void *a0, void *a1);
extern void k_reset(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void icon_init(void);
extern void icon_can_use_set(s32 a0, s32 a1);
extern void icon_cursor_move(s32 a0);
extern void parameter_show_init(void);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32 a0);
extern void shugaku_bank_ptr_init(void);
extern void sub_sub_step_up(void);

void shugaku_scene_init(void)
{
    s16 i;
    u32 bit;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    shared_system_state_reset();
    strcpy(ba_string, shugaku_scene_init_str0);
    k_reset(1);
    vblank_flag = 0;
    vb_ready = 1;
    screen_bri = 128;
    fade_bri = 128;
    bg_change_tbl2 = 0;
    hizuke_init();
    message_window_init();
    icon_init();
    for (i = 0; i < 15; i++) {
        icon_can_use_set(i, 0);
    }
    icon_cursor_move(1);
    parameter_show_init();
    parameter_show_init();
    fade_bri = 128;
    now_serifu_shugaku = 0;
    now_serifu_shugaku_in = 0;
    bustup_ptr_init();
    hizuke_init();
    yazirushi_off();
    date_name_set(0);
    strcpy(g_name, p_sei);
    shugaku_bank_ptr_init();
    ss_serifu_shugaku = shugaku_serifu_tbl[11];
    ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[11];
    sss_sound_shugaku = shugaku_sound_tbl[11];
    rpg_bat_arg = 255;
    date_place = 0;
    for (i = 0; i < 10; i++) {
        bit = ((u32)(*(s32 *)&screen_w[i * 56 + 456] << 30)) >> 31;
        girl_appear[i] = ((u32)bit < 1U) ^ 1;
    }
    girl_appear[10] = 0;
    basho_num = 0;
    sub_sub_step_up();
}
