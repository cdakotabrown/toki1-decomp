/* @SymbolStart: 0x801460A4 */
/* @SymbolSize: 124 */
/* Function: date_main3_serifu */
/* Overlay: DATE */
/* Address: 0x801460A4 (cross-prologue head) / 0x801460E8 (body) */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

extern void serifu_main(s32 arg0, s32 arg1, s32 arg2);

extern u8 date_now_serifu3[];
extern u8 date_now_serifu3_in[];
extern s32 date_ss_serifu3;
extern s32 date_ss_serifu3_kind;
extern s32 date_sss_sound3;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

extern u8 serifu_no;

void date_main3_serifu(void)
{
    now_s = (s32)date_now_serifu3;
    now_s_in = (s32)date_now_serifu3_in;
    ss_s = date_ss_serifu3;
    ss_s_kind = date_ss_serifu3_kind;
    sss_so = date_sss_sound3;
    serifu_main((s32)serifu_no, 1, 0);
}
