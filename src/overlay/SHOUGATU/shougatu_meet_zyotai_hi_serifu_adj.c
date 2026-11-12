/* @SymbolStart: 0x801345D0 */
/* Address: 0x801345D8 */
/* Size: 120 bytes (30 instructions) */
/* @PostProc: patch_insn_reg_field 0x34:rt:v1:t7 0x38:rs:v1:t7 0x40:rs:v1:t7 0x48:rs:v1:t7 0x4c:rt:t9:t0 0x50:rt:t7:t8 0x54:rs:t7:t8 0x54:rt:t7:t8 0x5c:rs:t7:t8 0x5c:rt:t8:t9 0x64:rt:t8:t9 0x68:rs:t9:t0 0x68:rt:t9:t0 0x70:rs:t9:t0 0x70:rt:t0:t1 0x74:rt:t0:t1 */
/* @SymbolSize: 128 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_alone_flag;

void shougatu_meet_zyotai_hi_serifu_adj(void)
{
    s32 t7;

    if (shougatu_alone_flag == 0) {
        t7 = get_g_zyotai_s(now_girl) & 0x7F;
        if (t7 != 0 && t7 != 1) {
            if (t7 != 2) {
                shougatu_meet_now_serifu += 2;
            } else {
                shougatu_meet_now_serifu += 1;
            }
        }
    }
    sub_sub_step_up();
}
