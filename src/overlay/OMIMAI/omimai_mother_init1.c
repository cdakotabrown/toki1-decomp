/* @SymbolSize: 160 */
/* /* @PostProc: rename_register_chain --chain t9:t8 --chain t0:t9 --chain t1:t0 --chain t2:t1 */
/* omimai_mother_init1 - OMIMAI overlay, Address: 0x801327BC, Size: 160 bytes (40 insns) */
#include "types.h"

extern u8 girl_no;
extern u8 now_girl;
extern s32 g_name;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 omimai_mother_init1_str0;
extern s32 omimai_serifu_res_tbl[][12];

extern void date_name_set(s32);
extern void strcpy(s32 *, s32);
extern void omimai_serifu_tbl_init(void);
extern void sub_sub_step_up(void);

void omimai_mother_init1(void)
{
    date_name_set(girl_no);
    now_girl = 14;
    strcpy(&g_name, (s32)&omimai_mother_init1_str0);
    now_serifu = ((u32)girl_no < 1U) ^ 1;
    now_serifu_in = 0;
    omimai_serifu_tbl_init();
    ss_serifu = omimai_serifu_res_tbl[0][11];
    ss_serifu_kind = omimai_serifu_res_tbl[1][11];
    sss_sound = omimai_serifu_res_tbl[2][11];
    sub_sub_step_up();
}
