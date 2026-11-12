/* @SymbolStart: 0x8013B720 */
/* @SymbolSize: 312 */
/* geko_girl2_ev7_scene_init -- GEKO overlay, Address: 0x8013B74C, Size: 268 bytes (67 insns) */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32 a0);
extern void shared_system_state_reset(void);
extern void strcpy(void *a0, void *a1);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern u8 now_girl;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;

extern u8 ba_string[];
extern u8 geko_scene_init4_str0[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl2_ev7_scene_init(void)
{
    now_year = 0x62;
    now_month = 2;
    now_day = 0x1C;
    now_youbi = 5;
    now_girl = 2;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    geko_ptr_tbl_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    date_name_set(now_girl);
    shared_system_state_reset();
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[20];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[20];
    geko_sss_soundev = geko_soundev_tbl[20];
    strcpy(&ba_string, &geko_scene_init4_str0);
    now_event_no = 20;
    sub_sub_step_up();
}
