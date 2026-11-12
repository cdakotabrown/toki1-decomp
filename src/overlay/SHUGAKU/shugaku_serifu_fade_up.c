/* @SymbolStart: 0x80138D70 */
/* @SymbolSize: 116 */
/* @PostProc: patch_insn_reg_field 0x44:rt:v0:t3 0x4c:rs:v0:t3 0x5c:rt:t3:t4 0x68:rt:t3:t4 */
/* Address: 0x80138D70 (prologue 0x80138D80), Size: 116 bytes (29 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void fade_up_main(void);

extern s32 *ss_serifu_shugaku_kind;
extern s16  now_serifu_shugaku;
extern s16  now_serifu_shugaku_in;
extern u8   shugaku_fade_skip;

void shugaku_serifu_fade_up(void)
{
    u8 val;

    val = *(u8 *)(ss_serifu_shugaku_kind[now_serifu_shugaku] + now_serifu_shugaku_in);

    if (val == 9) {
        sub_sub_step_up();
        shugaku_fade_skip = 1;
    } else {
        fade_up_main();
    }
}
