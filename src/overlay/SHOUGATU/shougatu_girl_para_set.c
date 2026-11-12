/* @SymbolStart: 0x80132E8C */
/* @SymbolSize: 260 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:t9 0x28:rs:v1:t9 0x28:rt:v1:t9 0x2c:rt:a2:a0 0x30:rs:v1:t9 0x30:rt:a2:a0 0x40:rs:a1:v1 0x40:rt:a1:v1 0x50:rt:a1:t3 0x54:rt:v1:t7 0x58:rd:t9:t0 0x5c:rs:a1:v1 0x5c:rt:t9:t0 0x70:rs:v1:t3 0x70:rt:v1:t3 0x78:rs:v1:t3 0x78:rt:a2:a0 0x7c:rd:t2:t4 0x80:rs:a1:v1 0x80:rt:t2:t4 0x8c:rs:t3:t5 0x8c:rt:t4:t6 0x94:rs:v1:t7 0x94:rt:v1:t7 0x9c:rs:v1:t7 0x9c:rt:a2:a0 0xa0:rd:t5:t8 0xa4:rs:a1:v1 0xa4:rt:t5:t8 0xbc:rt:v1:t1 0xc0:rs:v1:t1 0xc0:rt:v1:t1 0xc8:rs:v1:t1 0xcc:rt:a2:a0 0xd0:rs:v1:t1 0xd0:rt:a2:a0 0xd4:rt:a1:v1 0xd8:rs:a1:v1 0xd8:rt:a1:v1 0xdc:rd:t8:t2 0xe0:rs:a1:v1 0xe0:rt:t8:t2 0xe4:rt:t9:t3 0xec:rs:t9:t3 0xec:rt:t0:t4 0xf0:rt:t0:t4 0x60:rt:t0:t1 0x68:rs:t0:t1 0x68:rt:t1:t2 0x6c:rt:t1:t2 0x84:rt:t3:t5 0x90:rt:t4:t6 0xa8:rt:t6:t9 0xb0:rs:t6:t9 0xb0:rt:t7:t0 0xb8:rt:t7:t0 */
/* Address: 0x80132E94 */
/* Size: 252 bytes (63 instructions) */
#include "types.h"

extern s32 serifu_no;

extern void sub_sub_step_up(void);
extern void param_clamp(void);

extern volatile u8 now_girl;
extern volatile s16 shougatu_meet_now_serifu;
extern volatile u8 screen_w[];

void shougatu_girl_para_set(void)
{
    u8 charIdx;

    if (serifu_no != 0) {
        charIdx = now_girl;
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 1;
        *(s16 *)&screen_w[charIdx * 56 + 450] =
            *(s16 *)&screen_w[charIdx * 56 + 450] - 1;

        charIdx = now_girl;
        *(s16 *)&screen_w[charIdx * 56 + 446] =
            *(s16 *)&screen_w[charIdx * 56 + 446] - 1;

        charIdx = now_girl;
        *(s16 *)&screen_w[charIdx * 56 + 454] =
            *(s16 *)&screen_w[charIdx * 56 + 454] + 20;
    } else {
        charIdx = now_girl;
        if (charIdx == 0) {
            *(s16 *)&screen_w[charIdx * 56 + 446] =
                *(s16 *)&screen_w[charIdx * 56 + 446] + 1;
        }
    }

    param_clamp();
    sub_sub_step_up();
}
