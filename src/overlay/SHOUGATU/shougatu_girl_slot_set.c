/* @SymbolSize: 576 */
/* @RodataBase: 0x80143A50 */
#include "types.h"

extern u8 now_girl;

extern s32 shougatu_menu_ss_serifu;
extern s32 shougatu_menu_ss_serifu_kind;
extern s32 shougatu_menu_sss_sound;

extern s32 shougatu_menu_serifu_tbl[];
extern s32 shougatu_menu_serifu_kind_tbl[];
extern s32 shougatu_menu_sound_tbl[];

void shougatu_girl_slot_set(void)
{
    switch ((u32)now_girl) {
    case 1:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[5];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[5];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[5];
        break;
    case 2:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[2];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[2];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[2];
        break;
    case 3:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[3];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[3];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[3];
        break;
    case 4:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[9];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[9];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[9];
        break;
    case 5:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[6];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[6];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[6];
        break;
    case 6:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[7];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[7];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[7];
        break;
    case 7:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[4];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[4];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[4];
        break;
    case 8:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[0];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[0];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[0];
        break;
    case 9:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[8];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[8];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[8];
        break;
    case 10:
        shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[12];
        shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[12];
        shougatu_menu_sss_sound = shougatu_menu_sound_tbl[12];
        break;
    }
}
