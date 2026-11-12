/* @SymbolStart: 0x801324A8 */
/* date2_serifu_select -- DATE2 overlay, Address: 0x801324B0, Size: 208 bytes (52 insns) */
/* @SymbolSize: 216 */
#include "types.h"

extern s32 serifu_no;
extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s16 date2_g0_now_s;
extern s16 date2_g0_now_s_in;
extern s16 date2_girl_now_s;
extern s16 date2_girl_now_s_in;
extern s32 date2_girl_ptr[];
extern s32 date2_girl_res_tbl[][11];

void serifu_main(u8 arg0, s32 arg1, s32 arg2);

void date2_serifu_select(void)
{
    if (serifu_no != 0)
    {
        now_s = (s32)&date2_girl_now_s;
        now_s_in = (s32)&date2_girl_now_s_in;
        ss_s = date2_girl_ptr[0];
        ss_s_kind = date2_girl_ptr[1];
        sss_so = date2_girl_ptr[2];
    }
    else
    {
        now_s = (s32)&date2_g0_now_s;
        now_s_in = (s32)&date2_g0_now_s_in;
        ss_s = date2_girl_res_tbl[0][1];
        ss_s_kind = date2_girl_res_tbl[1][1];
        sss_so = date2_girl_res_tbl[2][1];
    }
    serifu_main(serifu_no, 1, 0);
}
