/* @SymbolSize: 80 */
/* @PostProc: swap_registers t6:t7 t7:t6 */
/* Address: 0x8013E534 */
/* Size: 80 bytes (20 instructions, +2-insn cross-prologue thunk for next func) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);
extern void shougatu_menu_serifu_run(void);
extern u8 now_girl;

void shougatu_zyotai4_menu_serifu_check(void)
{
    s32 v0;

    v0 = get_g_zyotai_s(now_girl);
    if ((v0 & 0x7F) != 4) {
        sub_sub_step_up();
        goto end;
    }
    shougatu_menu_serifu_run();
end:
    ;
}
