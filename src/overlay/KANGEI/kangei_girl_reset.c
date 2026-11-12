/* Address: 0x80138648, Size: 124 bytes (31 insns) */
/* @SymbolSize: 132 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 30 --to 42 */
#include "types.h"

extern volatile s32 serifu_no;
extern volatile u8 kangei_icon_flag;
extern volatile u8 now_girl;
extern s32 kangei_bank2_tbl_0_1;
extern s32 kangei_bank2_tbl_1_1;
extern s32 kangei_bank2_tbl_2_1;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern volatile s16 kangei_now_s;

extern void kangei_girl_init(void);
extern void date_name_set(s32);

void kangei_girl_reset(void)
{
    volatile s16 saved;

    saved = serifu_no;
    kangei_girl_init();
    serifu_no = saved;
    kangei_icon_flag = 0;
    now_girl = 0;
    date_name_set(0);
    kangei_ss_s = kangei_bank2_tbl_0_1;
    kangei_ss_s_kind = kangei_bank2_tbl_1_1;
    kangei_sss_so = kangei_bank2_tbl_2_1;
    kangei_now_s = 4;
}
