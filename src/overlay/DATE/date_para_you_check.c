/* @SymbolStart: 0x8013F4D0 */
/* @SymbolSize: 244 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0xd8 */
/* @PostProc: patch_insn_reg_field 0x40:rt:v1:t8 0x60:rt:a0:t0 0x64:rs:v1:t0 0x64:rt:v1:t0 0x68:rt:a1:a0 0x6c:rs:v1:t0 0x6c:rt:a1:a0 0x74:rs:a0:v1 0x74:rt:a0:v1 0x78:rt:v1:t4 0x80:rd:t0:t1 0x84:rs:a0:v1 0x84:rt:t0:t1 0x88:rt:t1:t2 0x90:rs:t1:t2 0x90:rt:t2:t3 0x94:rt:t2:t3 0x98:rs:v1:t4 0x98:rt:v1:t4 0xa0:rs:v1:t4 0xa0:rt:a1:a0 0xa4:rd:t3:t5 0xa8:rs:a0:v1 0xa8:rt:t3:t5 0xac:rt:t4:t6 0xb4:rs:t4:t6 0xb4:rt:t5:t7 0xb8:rt:t5:t7 0xbc:rs:v1:t8 0xbc:rt:v1:t8 0xc4:rs:v1:t8 0xc4:rt:a1:a0 0xc8:rd:t6:t9 0xcc:rs:a0:v1 0xcc:rt:t6:t9 0xd0:rt:t7:t0 0xd8:rs:t7:t0 0xd8:rt:t8:t1 0xe0:rt:t8:t1 */
/* Function: date_para_you_check */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 param_clamp();

extern s16 para_you;
extern volatile u8 now_girl;
extern u8 now_sub_sub_step;
extern s16 date_now_serifu_machi;
extern volatile s16 screen_w[][28];

void date_para_you_check(void)
{
    s32 sel;

    sel = para_you;
    if (sel < 30 || (sel < 35 && now_girl == 7)) {
        date_now_serifu_machi = 12;
        now_sub_sub_step = now_sub_sub_step + 15;
        screen_w[now_girl][223] -= 1;
        screen_w[now_girl][225] -= 1;
        screen_w[now_girl][227] += 5;
        param_clamp();
    } else {
        sub_sub_step_up();
    }
}
