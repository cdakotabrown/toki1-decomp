/* @SymbolStart: 0x8013BEC0 */
/* @RodataBase: 0x80143970 */
/* Address: 0x8013BED0 */
/* Size: 636 bytes (159 instructions) */
/* @SymbolSize: 652 */
#include "types.h"
extern s16 shougatu_appear_now_serifu;
extern u8 now_girl;

extern void sub_sub_step_up(void);

extern s32 shougatu_appear_ss_serifu;
extern s32 shougatu_appear_ss_serifu_kind;
extern s32 shougatu_appear_sss_sound;

extern s32 shougatu_appear_serifu_tbl[];
extern s32 shougatu_appear_serifu_kind_tbl[];
extern s32 shougatu_appear_sound_tbl[];

void shougatu_girl_serifu_set2(void)
{
    s32 t6;

    shougatu_appear_now_serifu = 0;
    t6 = now_girl;

switch (t6) {
    case 0:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[1];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[1];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[1];
        break;
    case 1:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[6];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[6];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[6];
        break;
    case 2:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[2];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[2];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[2];
        break;
    case 3:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[4];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[4];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[4];
        break;
    case 4:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[10];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[10];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[10];
        break;
    case 5:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[7];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[7];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[7];
        break;
    case 6:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[8];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[8];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[8];
        break;
    case 7:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[5];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[5];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[5];
        break;
    case 8:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[0];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[0];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[0];
        break;
    case 9:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[9];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[9];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[9];
        break;
    case 10:
        shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[12];
        shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[12];
        shougatu_appear_sss_sound = shougatu_appear_sound_tbl[12];
        break;
    }

    sub_sub_step_up();
}
