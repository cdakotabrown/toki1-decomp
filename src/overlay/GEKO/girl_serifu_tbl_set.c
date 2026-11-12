/* @SymbolStart: 0x801327AC */
/* @SymbolSize: 672 */
/* @RodataBase: 0x80144654 */
#include "types.h"
extern s16 geko_now_serifu;
extern s16 geko_now_serifu_in;
extern u8 now_girl;

extern void get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);

extern s32 geko_serifu_tbl[];
extern s32 geko_serifu_kind_tbl[];
extern s32 geko_sound_tbl[];
extern s32 geko_ss_serifu;
extern s32 geko_ss_serifu_kind;
extern s32 geko_sss_sound;

void girl_serifu_tbl_set(void)
{
    s32 sel;

    geko_now_serifu = 0;
    geko_now_serifu_in = 0;
    sel = now_girl;

switch (sel) {
    case 0:
        geko_ss_serifu = geko_serifu_tbl[1];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[1];
        geko_sss_sound = geko_sound_tbl[1];
        break;
    case 1:
        geko_ss_serifu = geko_serifu_tbl[5];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[5];
        geko_sss_sound = geko_sound_tbl[5];
        break;
    case 2:
        geko_ss_serifu = geko_serifu_tbl[2];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[2];
        geko_sss_sound = geko_sound_tbl[2];
        break;
    case 3:
        geko_ss_serifu = geko_serifu_tbl[3];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[3];
        geko_sss_sound = geko_sound_tbl[3];
        break;
    case 4:
        geko_ss_serifu = geko_serifu_tbl[9];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[9];
        geko_sss_sound = geko_sound_tbl[9];
        break;
    case 5:
        geko_ss_serifu = geko_serifu_tbl[6];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[6];
        geko_sss_sound = geko_sound_tbl[6];
        break;
    case 6:
        geko_ss_serifu = geko_serifu_tbl[7];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[7];
        geko_sss_sound = geko_sound_tbl[7];
        get_g_zyotai_s(6);
        break;
    case 7:
        geko_ss_serifu = geko_serifu_tbl[4];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[4];
        geko_sss_sound = geko_sound_tbl[4];
        break;
    case 8:
        geko_ss_serifu = geko_serifu_tbl[0];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[0];
        geko_sss_sound = geko_sound_tbl[0];
        break;
    case 9:
        geko_ss_serifu = geko_serifu_tbl[8];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[8];
        geko_sss_sound = geko_sound_tbl[8];
        break;
    case 10:
        geko_ss_serifu = geko_serifu_tbl[11];
        geko_ss_serifu_kind = geko_serifu_kind_tbl[11];
        geko_sss_sound = geko_sound_tbl[11];
        break;
    }
    sub_sub_step_up();
}
