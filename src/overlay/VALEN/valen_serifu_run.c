/* @SymbolStart: 0x80132348 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32, s32, s32);
extern u8 serifu_no;
extern s32 valen_ss_s;
extern s32 valen_ss_s_kind;
extern s32 valen_sss_so;
extern s32 valen_now_s[];
extern s32 valen_now_s_in[];
extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void valen_serifu_run(void)
{
    now_s = (s32)valen_now_s;
    now_s_in = (s32)valen_now_s_in;
    ss_s = valen_ss_s;
    ss_s_kind = valen_ss_s_kind;
    sss_so = valen_sss_so;
    serifu_main(serifu_no, 1, 0);
}
