/* @SymbolStart: 0x8013E584 */
/* @SymbolSize: 88 */
/* shougatu_girl9_appear_serifu_adj - SHOUGATU overlay, Address: 0x8013E58C, Size: 80 bytes (20 insns) */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x34:rt:v1:t0 0x38:rt:a0:t8 0x44:rt:a0:t8 0x4c:rt:v1:t0 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_menu_now_serifu;
extern u8 shougatu_girl9_appear;

void shougatu_girl9_appear_serifu_adj(void)
{
    s32 v, a, b;
    if (shougatu_girl9_appear != 0) {
        v = shougatu_menu_now_serifu;
        b = v + 2;
        a = v + 1;
        if (v == 1) {
            shougatu_menu_now_serifu = a;
        } else {
            shougatu_menu_now_serifu = b;
        }
    }
    sub_sub_step_up();
}
