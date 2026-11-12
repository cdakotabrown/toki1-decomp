/* geko_girl9_61_init -- GEKO overlay, Address: 0x8013EC00, Size: 284 bytes (71 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void class_clear_all(s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void strcpy(s32, s32);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(u32);
extern void shared_system_state_reset(void);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern u8 now_girl;
extern s8 cw3_disp;
extern s8 fade_bri;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;

extern s32 ba_string;
extern s32 geko_girl9_61_init_str0;
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl9_61_init(void)
{
    now_girl = 9;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy((s32)&ba_string, (s32)&geko_girl9_61_init_str0);
    cw3_disp = 0;
    (*(s8 *)&class_w[4].disp_flag) = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    geko_ptr_tbl_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    fade_bri = 0;
    date_name_set((u8)now_girl);
    shared_system_state_reset();
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[55];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[55];
    geko_sss_soundev = geko_soundev_tbl[55];
    now_event_no = 61;
    sub_sub_step_up();
}
