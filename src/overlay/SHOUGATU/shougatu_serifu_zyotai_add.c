/* Address: 0x801339E0 */
/* Size: 132 bytes (33 instructions) */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t6 0x2c:rs:v1:t6 0x34:rs:v1:t6 0x38:rt:t6:t7 0x40:rs:v1:t6 0x48:rs:v1:t6 0x4c:rt:t8:t9 0x58:rs:t6:t7 0x58:rt:t6:t7 0x60:rs:t6:t7 0x60:rt:t7:t8 0x68:rt:t7:t8 0x6c:rs:t8:t9 0x6c:rt:t8:t9 0x74:rs:t8:t9 0x74:rt:t9:t0 0x78:rt:t9:t0 */
/* @SymbolSize: 132 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s16 shougatu_meet_now_serifu;

void shougatu_serifu_zyotai_add(void)
{
    s32 t6;

    t6 = get_g_zyotai_s(now_girl) & 0x7F;
    switch (t6) {
    case 0:
        break;
    case 1:
    case 2:
        shougatu_meet_now_serifu += 1;
        break;
    case 3:
        shougatu_meet_now_serifu += 2;
        break;
    }
    sub_sub_step_up();
}
