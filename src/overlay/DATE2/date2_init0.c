/* @SymbolStart: 0x80132C60 */
/* @SymbolSize: 292 */
/* Address: 0x80132C78, Size: 268 bytes (67 insns) */
#include "types.h"

extern u8 day_flag;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;
extern u8 bg_change_tbl2;
extern char ba_string[];
extern char ba_string2[];
extern char date2_init0_str0[];
extern char date2_init0_str1[];
extern s32 date2_girl_res_tbl_0_10;
extern s32 date2_girl_res_tbl_1_10;
extern s32 date2_girl_res_tbl_2_10;
extern s32 date2_serifu_ptr[];
extern s16 date2_now_s;
extern s16 date2_now_s_in;
extern s16 date2_g0_now_s;
extern s16 date2_g0_now_s_in;
extern s16 date2_girl_now_s;
extern s16 date2_girl_now_s_in;

void text_sys_init(void);
void strcpy(void *arg0, void *arg1);
void k_reset(s32 arg0);
void shared_system_state_reset(void);
void bustup_ptr_init(void);
void hizuke_init(void);
void yazirushi_off(void);
void date_name_set(s32 arg0);
void date2_ptr_tbl_init(void);
void sub_sub_step_up(void);

void date2_init0(void)
{
    s32 t1, t2, t3;

    day_flag |= 0x10;
    vblank_flag = 0;
    vb_ready = 1;
    text_sys_init();
    strcpy(ba_string, date2_init0_str0);
    strcpy(ba_string2, date2_init0_str1);
    k_reset(1);
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 128;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    yazirushi_off();
    date_name_set(0);
    date2_ptr_tbl_init();
    t1 = date2_girl_res_tbl_0_10;
    t2 = date2_girl_res_tbl_1_10;
    t3 = date2_girl_res_tbl_2_10;
    date2_serifu_ptr[0] = t1;
    date2_serifu_ptr[1] = t2;
    date2_serifu_ptr[2] = t3;
    date2_now_s = 0;
    date2_now_s_in = 0;
    date2_g0_now_s = 0;
    date2_g0_now_s_in = 0;
    date2_girl_now_s = 0;
    date2_girl_now_s_in = 0;
    sub_sub_step_up();
}
