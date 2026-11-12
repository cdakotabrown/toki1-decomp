/* @RodataBase: 0x8014469C */
/* @PostProc: pad_trailing_nop --count 1 */
/* geko_girl_ptr_set -- GEKO overlay, Address: 0x80134C4C, 157 insns. */
#include "types.h"

extern u8 now_girl;

extern s32 geko_serifu1_tbl[];
extern s32 geko_serifu1_kind_tbl[];
extern s32 geko_sound1_tbl[];
extern s32 geko_ss_serifu_icon;
extern s32 geko_ss_serifu_icon_kind;
extern s32 geko_sss_sound_icon;

void geko_girl_ptr_set(void)
{
    switch (now_girl) {
    case 0:
        geko_ss_serifu_icon = geko_serifu1_tbl[1];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[1];
        geko_sss_sound_icon = geko_sound1_tbl[1];
        break;
    case 1:
        geko_ss_serifu_icon = geko_serifu1_tbl[6];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[6];
        geko_sss_sound_icon = geko_sound1_tbl[6];
        break;
    case 2:
        geko_ss_serifu_icon = geko_serifu1_tbl[3];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[3];
        geko_sss_sound_icon = geko_sound1_tbl[3];
        break;
    case 3:
        geko_ss_serifu_icon = geko_serifu1_tbl[4];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[4];
        geko_sss_sound_icon = geko_sound1_tbl[4];
        break;
    case 4:
        geko_ss_serifu_icon = geko_serifu1_tbl[10];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[10];
        geko_sss_sound_icon = geko_sound1_tbl[10];
        break;
    case 5:
        geko_ss_serifu_icon = geko_serifu1_tbl[7];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[7];
        geko_sss_sound_icon = geko_sound1_tbl[7];
        break;
    case 6:
        geko_ss_serifu_icon = geko_serifu1_tbl[8];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[8];
        geko_sss_sound_icon = geko_sound1_tbl[8];
        break;
    case 7:
        geko_ss_serifu_icon = geko_serifu1_tbl[5];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[5];
        geko_sss_sound_icon = geko_sound1_tbl[5];
        break;
    case 8:
        geko_ss_serifu_icon = geko_serifu1_tbl[0];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[0];
        geko_sss_sound_icon = geko_sound1_tbl[0];
        break;
    case 9:
        geko_ss_serifu_icon = geko_serifu1_tbl[9];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[9];
        geko_sss_sound_icon = geko_sound1_tbl[9];
        break;
    case 10:
        geko_ss_serifu_icon = geko_serifu1_tbl[12];
        geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[12];
        geko_sss_sound_icon = geko_sound1_tbl[12];
        break;
    }
}
