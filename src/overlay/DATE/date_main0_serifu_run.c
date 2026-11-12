/* @SymbolStart: 0x8013BC04 */
/* @SymbolSize: 120 */
/* Address: 0x8013BC04 (absorbed 17-word deferred-prologue head) */
/* Size: 120 bytes (30 insns) */
#include "types.h"

extern void serifu_main(s32 arg0, s32 arg1, s32 arg2);
extern u8 date_now_serifu0[];
extern u8 date_now_serifu0_in[];
extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 date_serifu0_tbl[];
extern s32 date_serifu0_kind_tbl[];
extern s32 date_sound0_tbl[];

void date_main0_serifu_run(void)
{
    now_s = (s32)date_now_serifu0;
    now_s_in = (s32)date_now_serifu0_in;
    ss_s = date_serifu0_tbl[1];
    ss_s_kind = date_serifu0_kind_tbl[1];
    sss_so = date_sound0_tbl[1];
    serifu_main(255, 1, 0);
}
