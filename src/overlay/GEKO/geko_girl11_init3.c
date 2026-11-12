/* geko_girl11_init3 -- GEKO overlay, Address: 0x801412DC, Size: 416 bytes (104 insns) */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void k_reset(s32 a0);
extern void bustup_ptr_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void icon_init(void);
extern void icon_can_use_set(s32 a0, s32 a1);
extern void icon_cursor_move(s32 a0);
extern void parameter_show_init(void);
extern void shared_system_state_reset(void);
extern void geko_ptr_tbl_init(void);
extern void strcpy(void *a0, void *a1);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern u8 now_girl;
extern s32 dec_bg_on;
extern u8 fade_bri;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;

extern u8 p_name[];
extern u8 g_name[];
extern u8 g_name2[];
extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 p_sei[];
extern u8 geko_girl11_init3_str0[];
extern u8 geko_girl11_init3_str1[];
extern u8 geko_girl11_init3_str2[];
extern u8 geko_girl11_init3_str3[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl11_init3(void)
{
    s16 i;

    now_girl = 11;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    k_reset(1);
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    icon_init();

    i = 0;
    do {
        icon_can_use_set(i, 0);
        i = i + 1;
    } while (i < 15);

    icon_cursor_move(1);
    parameter_show_init();
    parameter_show_init();
    fade_bri = 128;
    strcpy(&p_name, &p_sei);
    strcpy(&g_name, &geko_girl11_init3_str0);
    strcpy(&g_name2, &geko_girl11_init3_str1);
    shared_system_state_reset();
    dec_bg_on = 1;
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[77];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[77];
    geko_sss_soundev = geko_soundev_tbl[77];
    strcpy(&ba_string, &geko_girl11_init3_str2);
    strcpy(&ba_string2, &geko_girl11_init3_str3);
    sub_sub_step_up();
}
