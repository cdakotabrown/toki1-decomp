/* @SymbolStart: 0x801385E0 */
/* @SymbolSize: 96 */
#include "types.h"

extern void kangei_text_set(void);
extern void sub_sub_step_up(void);
extern s32 kangei_bank2_tbl_0_1;
extern s32 kangei_bank2_tbl_1_1;
extern s32 kangei_bank2_tbl_2_1;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern s16 kangei_now_s;

void kangei_text_step(void)
{
    kangei_ss_s = kangei_bank2_tbl_0_1;
    kangei_ss_s_kind = kangei_bank2_tbl_1_1;
    kangei_sss_so = kangei_bank2_tbl_2_1;
    kangei_now_s = 3;
    kangei_text_set();
    sub_sub_step_up();
}
