/* @SymbolSize: 624 */
/* @RodataBase: 0x801437A0 */
#include "types.h"

extern u8 now_girl;

extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

extern s32 shougatu_girl_serifu_tbl[];
extern s32 shougatu_girl_kind_tbl[];
extern s32 shougatu_girl_sound_tbl[];

void shougatu_serifu_set(void)
{
    switch ((u32)now_girl) {
    case 0:
        ss_serifu = shougatu_girl_serifu_tbl[1];
        ss_serifu_kind = shougatu_girl_kind_tbl[1];
        sss_sound = shougatu_girl_sound_tbl[1];
        break;
    case 1:
        ss_serifu = shougatu_girl_serifu_tbl[6];
        ss_serifu_kind = shougatu_girl_kind_tbl[6];
        sss_sound = shougatu_girl_sound_tbl[6];
        break;
    case 2:
        ss_serifu = shougatu_girl_serifu_tbl[3];
        ss_serifu_kind = shougatu_girl_kind_tbl[3];
        sss_sound = shougatu_girl_sound_tbl[3];
        break;
    case 3:
        ss_serifu = shougatu_girl_serifu_tbl[4];
        ss_serifu_kind = shougatu_girl_kind_tbl[4];
        sss_sound = shougatu_girl_sound_tbl[4];
        break;
    case 4:
        ss_serifu = shougatu_girl_serifu_tbl[10];
        ss_serifu_kind = shougatu_girl_kind_tbl[10];
        sss_sound = shougatu_girl_sound_tbl[10];
        break;
    case 5:
        ss_serifu = shougatu_girl_serifu_tbl[7];
        ss_serifu_kind = shougatu_girl_kind_tbl[7];
        sss_sound = shougatu_girl_sound_tbl[7];
        break;
    case 6:
        ss_serifu = shougatu_girl_serifu_tbl[8];
        ss_serifu_kind = shougatu_girl_kind_tbl[8];
        sss_sound = shougatu_girl_sound_tbl[8];
        break;
    case 7:
        ss_serifu = shougatu_girl_serifu_tbl[5];
        ss_serifu_kind = shougatu_girl_kind_tbl[5];
        sss_sound = shougatu_girl_sound_tbl[5];
        break;
    case 8:
        ss_serifu = shougatu_girl_serifu_tbl[0];
        ss_serifu_kind = shougatu_girl_kind_tbl[0];
        sss_sound = shougatu_girl_sound_tbl[0];
        break;
    case 9:
        ss_serifu = shougatu_girl_serifu_tbl[9];
        ss_serifu_kind = shougatu_girl_kind_tbl[9];
        sss_sound = shougatu_girl_sound_tbl[9];
        break;
    case 10:
        ss_serifu = shougatu_girl_serifu_tbl[12];
        ss_serifu_kind = shougatu_girl_kind_tbl[12];
        sss_sound = shougatu_girl_sound_tbl[12];
        break;
    }
}
