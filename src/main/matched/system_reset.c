/* Function: system_reset */
/* Address: 0x800414A0 */
/* Size: 676 bytes (169 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* 1. `@PostProc: patch_stack_complete --old 24 --new 40` could never fire */
/* 94.7%.  The +16 is the ugen PIC linkage area -- `@CompileFlags: */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* `@PostProc: custom_script custom_func_800414A0.py`, now retired and deleted. */
#include "types.h"

extern void ResetGraph(s32);
extern void vram_area_clear(s32);
extern void bzero(void *, s32);
extern void clear_brightness(void);
extern void tpage_buf_clear(void);
extern void search_tpage(s32, s32, s32);
extern void prim_init(void);
extern void class_clear_all(s32);
extern void dec_bg_reset(void);
extern void device_state_init(void);
extern void menu_bar_color(s32, s32, s32, s32);
extern void game_system_init(void);
extern void strncpy(void *, void *, s32);
extern void _game_work_init_set(void);
extern void card_w_clear(void);
extern void game_work_init2(void);

extern s16 screen_w;
extern s16 screen_h;
extern s16 screen_intmode;
extern s8 back_clear_r;
extern s8 back_clear_g;
extern s8 back_clear_b;
extern s16 disp_ofs_x[];
extern s16 disp_ofs_y[];
extern s32 frame_counter;
extern s32 step_frame_cnt;
extern s32 step_state;
extern s32 step_counter;
extern s8 now_step;
extern s8 now_sub_step;
extern s8 now_sub_sub_step;
extern s32 pad_trig;
extern s32 pad_prev;
extern s32 pad_data;
extern s8 pause_flag;
extern s8 sys_reset_req;
extern s32 vcount_lim;
extern s32 class_max;
extern s8 back_clear_flag;
extern s8 scene_sub_b;
extern s8 scene_sub_a;
extern s8 sd_proc_on;
extern s32 vb_ready;
extern u8 player_rec;
extern u8 p_sei;
extern u8 p_mei;
extern u8 p_adana;
extern union { u8 b; u16 h; } p_birthday;
extern u8 system_reset_str0;
extern u8 system_reset_str1;
extern u8 system_reset_str2;
extern u8 system_reset_str3;
extern u8 work_miura;
extern u8 work_tarao;
extern u8 work_takak;
extern u8 work_yamag;
extern u8 work_fuku;

void system_reset(s32 arg0) {
    ResetGraph(0);
    if (arg0 != 0) {
        vram_area_clear(255);
        bzero(&screen_w, 6796);
    }
    screen_w = 320;
    screen_h = 240;
    screen_intmode = 0;
    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    disp_ofs_x[0] = 160;
    disp_ofs_x[1] = 160;
    disp_ofs_y[0] = 360;
    disp_ofs_y[1] = 120;
    frame_counter = 0;
    step_frame_cnt = 0;
    step_state = 0;
    step_counter = 0;
    now_step = 16;
    now_sub_step = 0;
    now_sub_sub_step = 0;
    pad_trig = 0;
    pad_prev = 0;
    pad_data = 0;
    pause_flag = 0;
    sys_reset_req = 0;
    vcount_lim = 262;
    class_max = 159;
    clear_brightness();
    back_clear_flag = 0;
    scene_sub_b = 0;
    scene_sub_a = 0;
    sd_proc_on = 1;
    tpage_buf_clear();
    search_tpage(0x7FFF, 0, 0);
    vb_ready = 1;
    prim_init();
    class_clear_all(0);
    dec_bg_reset();
    device_state_init();
    menu_bar_color(16, 0, 0, 8);
    game_system_init();
    strncpy(&player_rec, &system_reset_str0, 16);
    strncpy(&p_sei, &system_reset_str1, 8);
    strncpy(&p_mei, &system_reset_str2, 8);
    strncpy(&p_adana, &system_reset_str3, 16);
    p_birthday.b = (p_birthday.b & 0xFFF0) | 1;
    p_birthday.h = (p_birthday.h & 0xFE0F) | 0x10;
    _game_work_init_set();
    card_w_clear();
    bzero(&work_miura, 256);
    bzero(&work_tarao, 256);
    bzero(&work_takak, 256);
    bzero(&work_yamag, 256);
    bzero(&work_fuku, 64);
    game_work_init2();
}
