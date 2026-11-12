/* @SymbolStart: 0x80138470 */
/* @SymbolSize: 188 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:t7 0x28:rs:v1:t7 0x28:rt:v1:t7 0x2c:rt:a1:a0 0x30:rs:v1:t7 0x30:rt:a1:a0 0x34:rt:a0:v1 0x38:rs:a0:v1 0x38:rt:a0:v1 0x3c:rt:v1:t1 0x40:rd:t7:t8 0x44:rs:a0:v1 0x44:rt:t7:t8 0x48:rt:t8:t9 0x50:rs:t8:t9 0x50:rt:t9:t0 0x54:rt:t9:t0 0x58:rs:v1:t1 0x58:rt:v1:t1 0x60:rs:v1:t1 0x60:rt:a1:a0 0x64:rd:t0:t2 0x68:rs:a0:v1 0x68:rt:t0:t2 0x6c:rt:t1:t3 0x74:rs:t1:t3 0x74:rt:t2:t4 0x7c:rt:t2:t4 0x80:rt:t4:t6 0x84:rs:t4:t6 0x84:rt:t4:t6 0x88:rt:t3:t5 0x90:rt:t3:t5 0x98:rs:t4:t6 0x98:rt:t5:t7 0xa0:rt:t5:t7 0xa4:rt:t6:t8 0xa8:rs:t6:t8 0xa8:rt:t6:t8 0xb0:rs:t6:t8 0xb0:rt:t7:t9 0xb8:rt:t7:t9 */
/* Address: 0x80138470 (cross-prologue head) / 0x80138478 (prologue) */
/* Size: 188 bytes (47 instructions), head included */
#include "types.h"

extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern volatile u8 now_girl;
extern s16 screen_w[][28];
extern volatile u8 now_sub_sub_step;
extern u8 kangei_icon_flag;
extern volatile s16 kangei_now_s;

void kangei_birth_choice_step(void)
{
    s32 t6 = serifu_no;
    s32 idx;

    if (t6 != 0) {
        idx = now_girl;
        screen_w[idx][225] = screen_w[idx][225] - 1;
        idx = now_girl;
        screen_w[idx][227] = screen_w[idx][227] + 5;
        param_clamp();
        kangei_icon_flag = 1;
        now_sub_sub_step = now_sub_sub_step + 56;
    } else {
        kangei_now_s = kangei_now_s + 1;
        sub_sub_step_up();
    }
}
