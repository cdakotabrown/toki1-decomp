/* @SymbolStart: 0x80137FCC */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32 arg0, s32 arg1, s32 arg2);
extern u8 date2_song_now_s[];
extern u8 date2_song_now_s_in[];
extern s32 date2_song_ptr[];
extern u8 *now_s;
extern u8 *now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern u8 serifu_no;

void date2_song_serifu_run(void)
{
    now_s = date2_song_now_s;
    now_s_in = date2_song_now_s_in;
    ss_s = date2_song_ptr[0];
    ss_s_kind = date2_song_ptr[1];
    sss_so = date2_song_ptr[2];
    serifu_main(serifu_no, 1, 0);
}
