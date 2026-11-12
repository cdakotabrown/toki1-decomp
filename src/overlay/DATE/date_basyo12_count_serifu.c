/* @SymbolStart: 0x8013BB8C */
/* @SymbolSize: 120 */
#include "types.h"

extern void serifu_main(s32 arg0, s32 arg1, s32 arg2);
extern u8 date_now_serifu_basyo12[];
extern u8 date_now_serifu_basyo12_in[];
extern u8 *now_s;
extern u8 *now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_basyo12_count_serifu(void)
{
    now_s = date_now_serifu_basyo12;
    now_s_in = date_now_serifu_basyo12_in;
    ss_s = date_serifu_tbl[9];
    ss_s_kind = date_serifu_kind_tbl[9];
    sss_so = date_sound_tbl[9];
    serifu_main(255, 1, 0);
}
