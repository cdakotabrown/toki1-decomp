/* @SymbolStart: 0x80142854 */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v1:t1 0x40:rs:v1:t1 0x40:rt:v1:t1 0x44:rt:a1:a0 0x48:rs:v1:t1 0x48:rt:a1:a0 0x54:rt:a0:v1 0x58:rs:a0:v1 0x58:rt:a0:v1 0x68:rt:v1:t5 0x6c:rd:t1:t2 0x70:rs:a0:v1 0x70:rt:t1:t2 0x74:rt:t2:t3 0x7c:rs:t2:t3 0x7c:rt:t3:t4 0x80:rt:t3:t4 0x84:rs:v1:t5 0x84:rt:v1:t5 0x8c:rs:v1:t5 0x8c:rt:a1:a0 0x90:rd:t4:t6 0x94:rs:a0:v1 0x94:rt:t4:t6 0x98:rt:t5:t7 0xa0:rs:t5:t7 0xa0:rt:t6:t8 0xa8:rt:t6:t8 */
/* Address: 0x8014285C */
/* Size: 172 bytes (43 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void param_clamp();

extern u16 now_serifuev;
extern volatile u8 now_girl;
extern u8 now_sub_sub_step;
extern u8 screen_w[];
extern s32 serifu_no;

void shougatu_taku_check4(void)
{
    if (serifu_no != 0) {
        now_sub_sub_step = now_sub_sub_step + 24;
        return;
    }
    now_serifuev = now_serifuev + 2;
    *(s16 *)(screen_w + now_girl * 56 + 446) += 1;
    *(s16 *)(screen_w + now_girl * 56 + 450) += 2;
    param_clamp();
    sub_sub_step_up();
}
