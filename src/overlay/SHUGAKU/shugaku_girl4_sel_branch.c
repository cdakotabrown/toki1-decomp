/* @SymbolStart: 0x801350B0 */
/* @SymbolSize: 204 */
/* @PostProc: patch_insn_reg_field 0x64:rt:t1:t2 0x68:rs:t1:t2 0x68:rt:t1:t2 0x70:rs:t1:t2 0x7c:rt:t3:t1 0x84:rt:t3:t1 0x8c:rd:t2:t3 0x90:rt:t2:t3 */
/* shugaku_girl4_sel_branch - SHUGAKU overlay, Address: 0x801350B8, Size: 196 bytes */
#include "types.h"

extern void event_log_add(s32 a0, s32 a1);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern u8  now_sub_sub_step;
extern s16 now_serifu_shugaku;
extern u8  menu_x;
extern volatile u8 now_girl;
extern u8  screen_w[];

void shugaku_girl4_sel_branch(void)
{
    u8 *q;

    if (serifu_no != 0) {
        now_sub_sub_step = (u8)(now_sub_sub_step + 29);
        now_serifu_shugaku = (s16)(now_serifu_shugaku + 1);
        return;
    }
    event_log_add(2, 32);
    sub_sub_step_up();
    q = screen_w + (s32)now_girl * 56;
    menu_x = 1;
    *(s16 *)(q + 446) = (s16)(*(s16 *)(q + 446) + 5);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 450) = (s16)(*(s16 *)(q + 450) + 2);
    param_clamp();
}
