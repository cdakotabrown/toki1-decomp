/* @SymbolStart: 0x80138978 */
/* @SymbolSize: 116 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t9:t0 0x34:rt:t1:t2 0x38:rs:t9:t0 0x38:rt:t0:t1 0x3c:rt:t0:t1 0x48:rt:t1:t2 */
#include "types.h"

extern void strcpy(s32 *, s32 *);
extern void sub_sub_step_up(void);
extern u8 now_girl;
extern s16 screen_w;
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern s32 ba_string;
extern s32 shougatu_girl2_basyo_set6_str0;

void shougatu_girl2_basyo_serifu23_set(void)
{
    s32 idx;

    idx = now_girl;
    *((u8 *)&screen_w + idx * 56 + 456) |= 4;
    shougatu_date_now_serifu = 23;
    shougatu_date_now_serifu_in = 0;
    strcpy(&ba_string, &shougatu_girl2_basyo_set6_str0);
    sub_sub_step_up();
}
