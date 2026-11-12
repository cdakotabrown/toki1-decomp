/* @SymbolStart: 0x80149D34 */
/* @SymbolSize: 124 */
/* Function: date_main2_serifu */
/* Overlay: DATE */
/* Address: 0x80149D34 (cross-prologue head) / 0x80149D78 (body) */
/* Size: 124 bytes (31 instructions) */
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

extern u8 serifu_no;

void date_main2_serifu(void)
{
    now_s = (s32)date_now_serifu2;
    now_s_in = (s32)date_now_serifu2_in;
    ss_s = date_ss_serifu2;
    ss_s_kind = date_ss_serifu2_kind;
    sss_so = date_sss_sound2;
    serifu_main((s32)serifu_no, 1, 0);
}
