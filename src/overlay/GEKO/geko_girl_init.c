/* @RodataBase: 0x801447D0 */
/* @SymbolStart: 0x80135D74 */
/* @SymbolSize: 2260 */
/* @CompileFlags: -cckr */
#include "types.h"

extern s32 geko_return_schedule(void);
extern s32 sub_sub_step_up(void);
extern s32 draw2d3d(s32, s32);
extern s32 class_switch(s32);
extern s32 prim_init(void);
extern s32 class_clear_all(s32);
extern s32 back_clear_switch(s32);
extern s32 icon_init(void);
extern s32 parameter_show_init(void);
extern s32 icon_can_use_set(s32, s32);
extern s32 icon_cursor_move(s32);
extern s32 text_sys_init(void);
extern s32 hizuke_init(void);
extern s32 message_window_init(void);
extern s32 tpage_buf_clear(void);
extern s32 strcpy(s32 *, s32 *);
extern s32 k_reset(s32);
extern s32 shared_system_state_reset(void);
extern s32 yazirushi_off(void);
extern s32 bustup_ptr_init(void);
extern s32 geko_ptr_table_init2(void);
extern s32 date_name_set(s32);
extern s32 ClearImage(s16 *, s32, s32, s32);
extern s32 reserved_date_check(s32, s32, s32);
extern s32 get_g_zyotai_s(s32);
extern s32 goto_sub_sub_step(s32);

extern s32 script_on;
extern u8  fade_bri;
extern u8  bg_change_tbl2;

extern u8  geko_sv_f10_flag;
extern u8  geko_dir_scroll_flag;
extern u8  geko_reserved_flag;
extern u8  geko_girl_flag;
extern s16 geko_voice_cnt;
extern s16 geko_voice_lim;
extern u8  geko_ans0_flag;
extern s16 geko_now_serifu2;
extern s16 geko_now_serifu2_in;
extern s32 geko_ss_serifu2;
extern s32 geko_ss_serifu2_kind;
extern s32 geko_sss_sound2;
extern u8  geko_gakunen;
extern u8  geko_wipe_flag;
extern s32 geko_voice_done;

extern s32 geko_serifu2_tbl[];
extern s32 geko_serifu2_kind_tbl[];
extern s32 geko_sound2_tbl[];

extern s32 geko_girl_init_str0, geko_girl_init_str1, geko_girl_init_str2;

extern s32 ba_string;
extern s32 ba_string2;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

typedef struct {
    u32 b0:6;
    u32 b6:1;
    u32 b7:13;
    u32 b20:4;
    u32 b24:8;
    u32 w4lo:25;
    u32 w4f:4;
    u32 w4hi:3;
    u8 pad[48];
} SaveChar;
typedef struct { u8 a; u8 b; u8 c; u8 d; } SaveTbl;
typedef struct {
    u8 pad0[58];
    u8 f3A;
    u8 pad3B[3];
    u8 year;
    u8 month;
    u8 day;
    u8 dow;
    u8 pad42[3];
    u8 arr69[32];
    u8 arr101[32];
    u8 pad85[111];
    u16 fF4lo:12;
    u16 fF4hi:4;
    u8 padF6[210];
    SaveChar chars[22];
    u8 pad698[4];
    SaveTbl tbl[32];
    u8 idx;
    u8 pad71D[1];
    u8 flags;
    u8 pad71F[2112];
    u8 curChar;
    u8 padF60[8];
    u8 fF68lo:4;
    u8 fF68hi:4;
    u8 padF69[313];
    u8 f10A2;
    u8 pad10A3[69];
    s32 f10E8;
    u8 pad10EC[12];
    s32 f10F8;
} SaveData;
extern SaveData screen_w;

#define SV screen_w
s32 geko_girl_init(void)
{
    s32 pad;
    s16 i;
    s32 d;
    s32 unused[7];
    u8  sp63;
    s16 rect[4];

    SV.curChar = SV.tbl[SV.idx].b;
    if ((SV.curChar == 0 && (SV.flags & 1)) ||
        (SV.curChar != 0 && (SV.flags & 2))) {
        geko_return_schedule();
        return 0;
    }
    if (!SV.chars[SV.curChar].b6 &&
        SV.curChar != 0) {
        geko_return_schedule();
        return 0;
    }
    geko_sv_f10_flag = (SV.flags & 0x10) > 0;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    icon_init();
    parameter_show_init();
    parameter_show_init();
    for (i = 0; i < 15; i++) {
        icon_can_use_set(i, 0);
    }
    icon_cursor_move(1);
    text_sys_init();
    geko_dir_scroll_flag = 0;
    geko_reserved_flag = 0;
    geko_girl_flag = 1;
    if (SV.month != 12 || SV.day != 24) {
        d = SV.day - 1;
        if (SV.dow != 6 &&
            SV.arr101[d] != 10 &&
            SV.arr101[d] != 16 &&
            SV.arr69[d] != 1 &&
            SV.arr69[d] != 2 &&
            SV.arr69[d] != 3 &&
            geko_sv_f10_flag == 0) {
            geko_girl_flag = 0;
        } else if (SV.fF68hi == 4 &&
                   SV.fF4hi == SV.chars[SV.curChar].b20) {
            if (reserved_date_check(SV.year, SV.month, SV.day) <= 0 ||
                SV.curChar != SV.chars[16].w4f) {
                geko_girl_flag = 0;
                geko_reserved_flag = 1;
            }
        }
    }
    geko_ans0_flag = 0;
    hizuke_init();
    message_window_init();
    tpage_buf_clear();
    strcpy(&ba_string, &geko_girl_init_str0);
    strcpy(&ba_string2, &geko_girl_init_str1);
    k_reset(1);
    shared_system_state_reset();
    yazirushi_off();
    script_on = 0;
    SV.f10A2 = 0;
    SV.f10E8 = 1;
    if (SV.curChar == 0) {
        SV.flags |= 1;
    } else {
        SV.flags |= 2;
    }
    SV.f3A = 128;
    fade_bri = 128;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    geko_voice_cnt = 0;
    geko_voice_lim = (u32)SV.f10F8 % 3 + 2;
    sp63 = get_g_zyotai_s(SV.curChar) & 0x7F;
    geko_now_serifu2 = 0;
    geko_now_serifu2_in = 0;
    now_serifu = (sp63 >= 2) + (sp63 >= 3) + (sp63 >= 4) + 30;
    now_serifu_in = 0;
    date_name_set(SV.curChar);
    geko_ptr_table_init2();
    switch (SV.curChar) {
    case 0:
        strcpy(&ba_string2, &geko_girl_init_str2);
        geko_ss_serifu2 = geko_serifu2_tbl[1];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[1];
        geko_sss_sound2 = geko_sound2_tbl[1];
        ss_serifu = geko_serifu2_tbl[1];
        ss_serifu_kind = geko_serifu2_kind_tbl[1];
        sss_sound = geko_sound2_tbl[1];
        break;
    case 1:
        geko_ss_serifu2 = geko_serifu2_tbl[5];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[5];
        geko_sss_sound2 = geko_sound2_tbl[5];
        break;
    case 2:
        geko_ss_serifu2 = geko_serifu2_tbl[2];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[2];
        geko_sss_sound2 = geko_sound2_tbl[2];
        ss_serifu = geko_serifu2_tbl[2];
        ss_serifu_kind = geko_serifu2_kind_tbl[2];
        sss_sound = geko_sound2_tbl[2];
        break;
    case 3:
        geko_ss_serifu2 = geko_serifu2_tbl[3];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[3];
        geko_sss_sound2 = geko_sound2_tbl[3];
        break;
    case 4:
        geko_ss_serifu2 = geko_serifu2_tbl[10];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[10];
        geko_sss_sound2 = geko_sound2_tbl[10];
        break;
    case 5:
        geko_ss_serifu2 = geko_serifu2_tbl[6];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[6];
        geko_sss_sound2 = geko_sound2_tbl[6];
        break;
    case 6:
        geko_ss_serifu2 = geko_serifu2_tbl[8];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[8];
        geko_sss_sound2 = geko_sound2_tbl[8];
        ss_serifu = geko_serifu2_tbl[8];
        ss_serifu_kind = geko_serifu2_kind_tbl[8];
        sss_sound = geko_sound2_tbl[8];
        if (sp63 < 2) {
            geko_ss_serifu2 = geko_serifu2_tbl[7];
            geko_ss_serifu2_kind = geko_serifu2_kind_tbl[7];
            geko_sss_sound2 = geko_sound2_tbl[7];
            ss_serifu = geko_serifu2_tbl[7];
            ss_serifu_kind = geko_serifu2_kind_tbl[7];
            sss_sound = geko_sound2_tbl[7];
        }
        break;
    case 7:
        geko_ss_serifu2 = geko_serifu2_tbl[4];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[4];
        geko_sss_sound2 = geko_sound2_tbl[4];
        break;
    case 8:
        geko_ss_serifu2 = geko_serifu2_tbl[0];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[0];
        geko_sss_sound2 = geko_sound2_tbl[0];
        break;
    case 9:
        geko_ss_serifu2 = geko_serifu2_tbl[9];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[9];
        geko_sss_sound2 = geko_sound2_tbl[9];
        break;
    case 10:
        geko_ss_serifu2 = geko_serifu2_tbl[12];
        geko_ss_serifu2_kind = geko_serifu2_kind_tbl[12];
        geko_sss_sound2 = geko_sound2_tbl[12];
        break;
    }
    if (SV.month >= 4) {
        geko_gakunen = SV.year - 95;
    } else {
        geko_gakunen = SV.year - 96;
    }
    geko_wipe_flag = 0;
    geko_voice_done = 0;
    if (((SV.year | 0) == 95 && (SV.month | 0) == 6 && SV.day == 3) ||
        ((SV.year | 0) == 96 && (SV.month | 0) == 6 && SV.day == 1) ||
        ((SV.year | 0) == 97 && (SV.month | 0) == 6 && SV.day == 7) ||
        ((SV.year | 0) == 95 && (SV.month | 0) == 10 && SV.day == 14) ||
        ((SV.year | 0) == 96 && (SV.month | 0) == 10 && SV.day == 12) ||
        ((SV.year | 0) == 97 && (SV.month | 0) == 10 && SV.day == 11)) {
        rect[0] = 320;
        rect[1] = 416;
        rect[2] = 320;
        rect[3] = 88;
        ClearImage(rect, 8, 0, 0);
        rect[0] = 704;
        rect[1] = 160;
        ClearImage(rect, 8, 0, 0);
    }
    sub_sub_step_up();
    if (geko_sv_f10_flag != 0 || (SV.flags & 0x20)) {
        goto_sub_sub_step(67);
    }
    if (geko_girl_flag != 0) {
        goto_sub_sub_step(67);
    }
}
