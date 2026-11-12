/* @SymbolStart: 0x80136928 */
/* @SymbolSize: 164 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: patch_insn_reg_field 0x3c:rt:t9:t5 0x40:rs:t9:t5 0x40:rt:t9:t5 0x48:rt:t2:v0 0x50:rt:t0:t6 0x54:rs:t2:v0 0x54:rt:t2:v0 0x58:rs:t0:t6 0x58:rt:t0:t6 0x5c:rt:t9:t5 0x60:rt:t4:t2 0x68:rt:t1:t7 0x6c:rs:t4:t2 0x6c:rt:t4:t2 0x70:rs:t1:t7 0x70:rt:t1:t7 0x74:rd:t3:t9 0x74:rt:t2:v0 0x78:rt:t0:t6 0x80:rd:t3:t1 0x80:rs:t3:t9 0x80:rt:t2:v0 0x88:rd:t5:v0 0x88:rs:t3:t1 0x88:rt:t4:t2 0x8c:rt:t1:t7 0x94:rs:t5:v0 0x94:rt:t6:t4 0xa0:rt:t6:t4 */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 now_girl;
extern u8 now_year;
extern s32 shougatu_serifu_save_ptr[];
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s16 now_serifu;
extern s32 shougatu_pack_serifu_tbl[];
extern s32 shougatu_pack_kind_tbl[];
extern s32 shougatu_pack_sound_tbl[];

void shougatu_serifu_swap(void)
{
    shougatu_serifu_save_ptr[0] = ss_serifu;
    shougatu_serifu_save_ptr[1] = ss_serifu_kind;
    shougatu_serifu_save_ptr[2] = sss_sound;
    ss_serifu = shougatu_pack_serifu_tbl[11];
    ss_serifu_kind = shougatu_pack_kind_tbl[11];
    sss_sound = shougatu_pack_sound_tbl[11];
    now_serifu = (s16)(now_girl * 3 + now_year - 96);
    sub_sub_step_up();
}
