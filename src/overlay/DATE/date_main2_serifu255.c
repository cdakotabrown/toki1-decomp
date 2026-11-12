/* @SymbolStart: 0x80149E40 */
/* @SymbolSize: 120 */
/* Function: date_main2_serifu255 */
/* Overlay: DATE */
/* Address: 0x80149E40 (cross-prologue head) / 0x80149E84 (body) */
/* Size: 120 bytes (30 instructions) */
#include "types.h"

extern void serifu_main(s32 arg0, s32 arg1, s32 arg2);

extern u8 date_now_serifu2[];
extern u8 date_now_serifu2_in[];
extern s32 date_ss_serifu2;
extern s32 date_ss_serifu2_kind;
extern s32 date_sss_sound2;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void date_main2_serifu255(void)
{
    now_s = (s32)date_now_serifu2;
    now_s_in = (s32)date_now_serifu2_in;
    ss_s = date_ss_serifu2;
    ss_s_kind = date_ss_serifu2_kind;
    sss_so = date_sss_sound2;
    serifu_main(255, 1, 0);
}
