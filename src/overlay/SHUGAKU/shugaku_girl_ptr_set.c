/* @SymbolSize: 624 */
/* @RodataBase: 0x8013ACE0 */
/* shugaku_girl_ptr_set - SHUGAKU overlay, Address: 0x80133A58, Size: 624 bytes (156 insns) */
#include "types.h"

extern u8 now_girl;

extern u32 shugaku_pbirth_ss_serifu;
extern u32 shugaku_pbirth_ss_serifu_kind;
extern u32 shugaku_pbirth_sss_sound;

extern s32 shugaku_pbirth_serifu_tbl[];
extern s32 shugaku_pbirth_serifu_kind_tbl[];
extern s32 shugaku_pbirth_sound_tbl[];

void shugaku_girl_ptr_set(void)
{
    switch (now_girl) {
    case 0:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[1]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[1]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[1]);
        break;
    case 1:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[6]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[6]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[6]);
        break;
    case 2:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[3]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[3]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[3]);
        break;
    case 3:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[4]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[4]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[4]);
        break;
    case 4:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[10]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[10]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[10]);
        break;
    case 5:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[7]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[7]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[7]);
        break;
    case 6:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[8]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[8]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[8]);
        break;
    case 7:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[5]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[5]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[5]);
        break;
    case 8:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[0]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[0]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[0]);
        break;
    case 9:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[9]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[9]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[9]);
        break;
    case 10:
        shugaku_pbirth_ss_serifu = (*(u32 *)&shugaku_pbirth_serifu_tbl[12]);
        shugaku_pbirth_ss_serifu_kind = (*(u32 *)&shugaku_pbirth_serifu_kind_tbl[12]);
        shugaku_pbirth_sss_sound = (*(u32 *)&shugaku_pbirth_sound_tbl[12]);
        break;
    }
}
