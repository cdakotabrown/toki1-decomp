/* @SymbolStart: 0x8014C4B8 */
/* @SymbolSize: 120 */
/* Function: date_serifuev255_main */
/* Overlay: DATE */
/* Address: 0x8014C4B8 (cross-prologue head) / 0x8014C4FC (body) */
/* Size: 120 bytes (30 instructions) */
#include "types.h"

extern void serifu_main(s32 arg0, s32 arg1, s32 arg2);

extern u8 now_serifuev[];
extern u8 now_serifuev_in[];
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void date_serifuev255_main(void)
{
    now_s = (s32)now_serifuev;
    now_s_in = (s32)now_serifuev_in;
    ss_s = date_ss_serifuev;
    ss_s_kind = date_ss_serifuev_kind;
    sss_so = date_sss_soundev;
    serifu_main(255, 1, 0);
}
