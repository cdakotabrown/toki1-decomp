/* @SymbolStart: 0x8014C530 */
/* @SymbolSize: 124 */
/* Function: date_serifuev_main */
/* Overlay: DATE */
/* Address: 0x8014C530 (cross-prologue head) / 0x8014C574 (body) */
/* Size: 124 bytes (31 instructions) */
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

extern u8 serifu_no;

void date_serifuev_main(void)
{
    now_s = (s32)now_serifuev;
    now_s_in = (s32)now_serifuev_in;
    ss_s = date_ss_serifuev;
    ss_s_kind = date_ss_serifuev_kind;
    sss_so = date_sss_soundev;
    serifu_main((s32)serifu_no, 1, 0);
}
