/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:8 IDO52_AS1_TIE_TAKE=36 IDO52_AS1_TIE_REJECT=43,44 */
/* @CompileFlags: -cckr */
#include "types.h"

extern unsigned char vblank_flag;
extern int vb_ready;
extern unsigned char fade_bri;
extern short kangei_now_s;
extern short kangei_now_s_in;
extern unsigned char now_girl;
extern volatile unsigned short basyo_pack;
struct Flags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int rest : 30;
};
struct SFlags {
    int b0 : 1;
    int b1 : 1;
    int rest : 30;
};
extern unsigned char screen_w[];
extern short girl10_tokimeki;
extern short girl10_yuukou;
extern short girl0_yuukou;
extern short girl0_tokimeki;
extern unsigned char now_year;
extern int kangei_ss_s;
extern int kangei_ss_s_kind;
extern int kangei_sss_so;
extern unsigned char kangei_icon_flag;
extern s32 kangei_msg_res_tbl[][8];

void kangei_girl_init()
{
    unsigned int slot;
    short i;
    unsigned char *rec;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    back_clear_switch(1);
    class_clear_all(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_sys_init();
    vblank_flag = 0;
    vb_ready = 1;
    fade_bri = 0;
    shared_system_state_reset();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    kangei_now_s = 0;
    kangei_now_s_in = 0;
    bustup_ptr_init();
    yazirushi_off();
    now_girl = 255;
    slot = basyo_pack;
    slot >>= 12;
    for (i = 0; i < 11; i++) {
        rec = screen_w + i * 56;
        if (slot == rec[458] >> 4 && ((struct SFlags *)(rec + 456))->b1) {
            now_girl = i;
            break;
        }
    }
    if (slot == 10 && girl0_tokimeki + girl0_yuukou < girl10_yuukou + girl10_tokimeki &&
        now_year >= 96) {
        now_girl = 10;
    }
    date_name_set(now_girl);
    kangei_ptr_tbl2_init();
    kangei_ss_s = (*(int *)&kangei_msg_res_tbl[0][6]);
    kangei_ss_s_kind = (*(int *)&kangei_msg_res_tbl[1][6]);
    kangei_sss_so = (*(int *)&kangei_msg_res_tbl[2][6]);
    kangei_icon_flag = 0;
    sub_sub_step_up();
}
