/* @SymbolStart: 0x80132A34 */
/* @RodataBase: 0x80143628 */
/* Address: 0x80132A3C */
/* Size: 704 bytes (176 insns), head 0x80132A34 / prologue 0x80132A3C */
/* @SymbolSize: 704 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);

extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;

extern u8 now_girl;
extern s32 shougatu_meet_serifu_tbl[];
extern s32 shougatu_meet_serifu_kind_tbl[];
extern s32 shougatu_meet_sound_tbl[];

void shougatu_girl_serifu_set(void) {
    u8 r;

    switch (now_girl) {
    case 0:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[2];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[2];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[2];
        break;
    case 1:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[6];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[6];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[6];
        break;
    case 2:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[3];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[3];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[3];
        break;
    case 3:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[4];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[4];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[4];
        break;
    case 4:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[11];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[11];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[11];
        break;
    case 5:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[7];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[7];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[7];
        break;
    case 6:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[9];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[9];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[9];
        r = get_g_zyotai_s(6);
        if ((u32)(r & 0x7F) < 2) {
            shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[8];
            shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[8];
            shougatu_meet_sss_sound = shougatu_meet_sound_tbl[8];
        }
        break;
    case 7:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[5];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[5];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[5];
        break;
    case 8:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[1];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[1];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[1];
        break;
    case 9:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[10];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[10];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[10];
        break;
    case 10:
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[13];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[13];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[13];
        break;
    }
}
