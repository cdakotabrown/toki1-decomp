/* Address: 0x80140CA8 */
/* Size: 344 bytes (86 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void k_reset(s32);
extern void mes_window_switch(s32);
extern void shougatu_ptr_set2(void);

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u8 cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 cal_cw_clut;
extern s32 shougatu_menu_ss_serifu;
extern s32 shougatu_menu_ss_serifu_kind;
extern s32 shougatu_menu_sss_sound;
extern s16 shougatu_menu_now_serifu;
extern s32 shougatu_menu_bg_res[];
extern s32 shougatu_menu_serifu_tbl[];
extern s32 shougatu_menu_serifu_kind_tbl[];
extern s32 shougatu_menu_sound_tbl[];

void shougatu_state_init(void)
{
    mes_window_switch(0);
    k_reset(1);
    shougatu_ptr_set2();
    palette_upload_enqueue(shougatu_menu_bg_res[4], 17, 1, 2, 0);
    class_clear_n(96);
    cal_cw_prio = 1;
    cal_cw_mode = 0;
    cal_cw_attr = 0x01000000;
    cal_cw_disp = 4;
    cal_cw_img = shougatu_menu_bg_res[1];
    cal_cw_anim_tbl = shougatu_menu_bg_res[2];
    cal_cw_hdr = shougatu_menu_bg_res[0];
    cal_cw_aux14 = (*(s16 *)&shougatu_menu_bg_res[3]);
    cal_cw_anim = 0;
    cal_cw_frame = 0;
    cal_cw_rate = 1;
    cal_cw_clut = 17;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_bright = 0;
    cal_cw_sel = 0;
    cal_cw_flag4 = 0;
    shougatu_menu_now_serifu = 19;
    shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[11];
    shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[11];
    shougatu_menu_sss_sound = shougatu_menu_sound_tbl[11];
    sub_sub_step_up();
}
