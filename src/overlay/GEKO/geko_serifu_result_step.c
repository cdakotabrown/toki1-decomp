/* @SymbolStart: 0x80137454 */
/* @SymbolSize: 184 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:a1:a0 0x30:rt:a1:a0 0x34:rt:a0:v1 0x38:rs:a0:v1 0x38:rt:a0:v1 0x44:rs:a0:v1 0x60:rt:a1:a0 0x68:rs:a0:v1 0x6c:rt:v1:t3 0x74:rs:v1:t3 */
/* Address: 0x8013745C  Size: 176 bytes (44 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void param_clamp();
extern void geko_goto_sub31();

extern volatile u8 now_girl;
extern u8 screen_w[][56];
extern u8 geko_ans0_flag;
extern s16 geko_now_serifu2;
extern s32 serifu_no;

void geko_serifu_result_step(void) {
    s32 t6 = serifu_no;

    u8 *v0;
    if (t6 != 0) {
        v0 = screen_w[now_girl];
        *(s16 *)(v0 + 450) = *(s16 *)(v0 + 450) - 1;
        v0 = screen_w[now_girl];
        {
            s16 val = *(s16 *)(v0 + 454) + 5;
            *(s16 *)(v0 + 454) = val;
            param_clamp();
        }
        geko_ans0_flag = 0;
        geko_goto_sub31();
    } else {
        s16 t = geko_now_serifu2;
        geko_ans0_flag = 1;
        geko_now_serifu2 = t + 1;
        sub_sub_step_up();
    }
}
