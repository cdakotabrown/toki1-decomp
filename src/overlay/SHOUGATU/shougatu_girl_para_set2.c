/* @SymbolSize: 132 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v0:a1 0x2c:rs:v0:a1 0x2c:rt:v0:a1 0x30:rt:a3:a0 0x34:rs:v0:a1 0x34:rt:a3:a0 0x38:rt:a2:v1 0x3c:rs:a2:v1 0x3c:rt:a2:v1 0x44:rd:v1:v0 0x44:rs:a2:v1 0x48:rs:v1:v0 0x4c:rs:v0:a1 0x4c:rt:a3:a0 0x54:rs:v1:v0 0x5c:rd:a0:v0 0x5c:rs:a2:v1 0x60:rs:a0:v0 0x70:rs:a0:v0 */
/* Address: 0x80134738 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void date_name_set();
extern void param_clamp();
extern u8 serifu_no;
extern u8 screen_w[];
extern u8 now_girl;

void shougatu_girl_para_set2(void)
{
    s32 idx;
    s32 idx2;

    date_name_set(serifu_no);
    idx = *(s32 *)&serifu_no;
    *(s16 *)&screen_w[idx * 56 + 446] =
        *(s16 *)&screen_w[idx * 56 + 446] + 5;
    idx2 = *(s32 *)&serifu_no;
    *(s16 *)&screen_w[idx2 * 56 + 450] =
        *(s16 *)&screen_w[idx2 * 56 + 450] + 1;
    param_clamp();
    now_girl = 14;
    sub_sub_step_up();
}
