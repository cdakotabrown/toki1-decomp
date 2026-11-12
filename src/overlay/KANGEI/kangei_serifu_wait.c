/* @SymbolStart: 0x80134004 */
/* @SymbolSize: 144 */
#include "types.h"

extern s32 serifu_main(s32, s32, s32);
extern void sub_sub_step_up(void);
extern u8 kangei_serifu_mode;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern s32 kangei_now_s[];
extern s32 kangei_now_s_in[];
extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void kangei_serifu_wait(void)
{
    now_s = (s32)kangei_now_s;
    now_s_in = (s32)kangei_now_s_in;
    ss_s = kangei_ss_s;
    ss_s_kind = kangei_ss_s_kind;
    sss_so = kangei_sss_so;
    if (serifu_main(255, 1, kangei_serifu_mode) == 1) {
        sub_sub_step_up();
    }
}
