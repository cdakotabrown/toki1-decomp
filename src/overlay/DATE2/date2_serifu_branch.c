/* @SymbolStart: 0x8013775C */
/* @SymbolSize: 176 */
/* @PostProc: patch_insn_reg_field 0x34:rt:a1:v1 0x38:rs:a1:v1 0x38:rt:a1:v1 0x3c:rt:t0:t1 0x40:rd:v1:t8 0x44:rs:a1:v1 0x44:rt:v1:t8 0x48:rt:t8:t9 0x50:rs:t8:t9 0x50:rt:t9:t0 0x54:rt:t9:t0 0x58:rs:t0:t1 0x58:rt:t0:t1 0x60:rs:t0:t1 0x64:rd:v1:t2 0x68:rs:a1:v1 0x68:rt:v1:t2 0x6c:rt:t1:t3 0x74:rs:t1:t3 0x74:rt:t2:t4 0x7c:rt:t2:t4 0x80:rt:t3:t5 0x84:rs:t3:t5 0x84:rt:t3:t5 0x8c:rs:t3:t5 0x8c:rt:t4:t6 0x94:rt:t4:t6 0x98:rt:t5:t7 0x9c:rs:t5:t7 0x9c:rt:t5:t7 0xa4:rs:t5:t7 0xa4:rt:t6:t8 0xac:rt:t6:t8 */
/* Address: 0x80137764, Size: 168 bytes (42 insns) */
#include "types.h"

extern volatile u8 now_girl;
extern volatile u8 now_sub_sub_step;
extern volatile s16 now_serifu;
extern u8 screen_w[];
extern s32 serifu_no;

void param_clamp(void);
void sub_sub_step_up(void);

void date2_serifu_branch(void)
{
    s32 t6;
    u32 off;

    t6 = serifu_no;

    if (t6 != 0) {
        off = (u32)now_girl * 56;
        *(s16 *)(screen_w + off + 450) =
            *(s16 *)(screen_w + off + 450) - 1;

        off = (u32)now_girl * 56;
        *(s16 *)(screen_w + off + 454) =
            *(s16 *)(screen_w + off + 454) + 5;

        param_clamp();
        now_sub_sub_step = now_sub_sub_step + 13;
    } else {
        now_serifu = now_serifu + 1;
        sub_sub_step_up();
    }
}
