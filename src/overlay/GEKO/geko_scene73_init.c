/* geko_scene73_init -- GEKO overlay, Address: 0x80140464, Size: 464 bytes (116 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void basyo_init(void *a0);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void icon_init(void);
extern void icon_can_use_set(s32 a0, s32 a1);
extern void icon_cursor_move(s32 a0);
extern void parameter_show_init(void);
extern void date_name_set(s32 a0);
extern void shared_system_state_reset(void);
extern void strcpy(void *a0, void *a1);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

typedef struct {
    s8 _pad[56];
} CharRec;

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern u8 now_girl;
extern s8 cw3_disp;
extern s32 dec_bg_on;
extern u8 fade_bri;
extern s8 geko_fill_on;
extern s32 geko_ptc_cnt;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;

extern CharRec screen_w[];
extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 geko_scene73_basyo_str[];
extern u8 geko_scene73_init_str0[];
extern u8 geko_scene73_init_str1[];
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_scene73_init(void)
{
    s16 i;
    s16 *counter;

    now_girl = 10;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    basyo_init(&geko_scene73_basyo_str);
    cw3_disp = 0;
    (*(s8 *)&class_w[4].disp_flag) = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    geko_ptr_tbl_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    dec_bg_on = 1;
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
    geko_fill_on = 0;
    geko_ptc_cnt = 0;
    date_name_set(now_girl);
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[73];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[73];
    geko_sss_soundev = geko_soundev_tbl[73];
    strcpy(&ba_string, &geko_scene73_init_str0);
    strcpy(&ba_string2, &geko_scene73_init_str1);

    counter = (s16 *)((s8 *)&screen_w[now_girl] + 450);
    *counter = *counter + 2;
    param_clamp();
    now_event_no = 73;
    sub_sub_step_up();
}
