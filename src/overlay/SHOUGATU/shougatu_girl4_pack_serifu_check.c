/* @CompileEnv: IDO52_COPYPROP_VAR=-8 IDO52_FORCE_WEB_COLOR=10:v1 */
/* @SymbolStart: 0x8013AF0C */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x14:rt:t7:t6 0x1c:rs:t7:t6 0x1c:rt:t7:t6 0x24:rd:t6:v0 0x28:rd:t8:t7 0x28:rt:t7:t6 0x2c:rs:t6:t7 0x2c:rt:t8:v0 0x38:rs:t6:v0 0x3c:rt:t0:t9 0x40:rt:t9:t8 0x4c:rt:t9:t8 0x54:rt:t0:t9 */
/* Address: 0x8013AF1C */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern u8  girl4_basyo;
extern u16 basyo_pack;

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;

void shougatu_girl4_pack_serifu_check(void)
{
    s32 v1;
    s32 v0;

    v1 = girl4_basyo;
    v0 = (u32)v1 >> 4;
    if (((u32)basyo_pack >> 12) != v0) goto call;
    if (v0 != 7) goto store8;
    shougatu_date_now_serifu = 10;
    goto call;
store8:
    shougatu_date_now_serifu = 8;
call:
    sub_sub_step_up();
}
