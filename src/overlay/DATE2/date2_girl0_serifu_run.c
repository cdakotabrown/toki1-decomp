/* @SymbolStart: 0x801323B0 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32 arg0, s32 arg1, s32 arg2);
extern u8 date2_g0_now_s[];
extern u8 date2_g0_now_s_in[];
extern u8 *now_s;
extern u8 *now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern u8 serifu_no;
extern s32 date2_girl_res_tbl[][11];

void date2_girl0_serifu_run(void)
{
    now_s = date2_g0_now_s;
    now_s_in = date2_g0_now_s_in;
    ss_s = date2_girl_res_tbl[0][1];
    ss_s_kind = date2_girl_res_tbl[1][1];
    sss_so = date2_girl_res_tbl[2][1];
    serifu_main(serifu_no, 1, 0);
}
