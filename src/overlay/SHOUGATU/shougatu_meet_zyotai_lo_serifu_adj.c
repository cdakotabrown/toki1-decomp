/* @SymbolStart: 0x80134650 */
/* Address: 0x80134658 */
/* Size: 152 bytes (38 instructions) */
/* @PostProc: patch_insn_reg_field 0x34:rt:v1:t7 0x38:rs:v1:t7 0x40:rs:v1:t7 0x48:rs:v1:t7 0x4c:rt:t9:t0 0x58:rt:t7:t8 0x5c:rs:t7:t8 0x5c:rt:t7:t8 0x64:rs:t7:t8 0x64:rt:t8:t9 0x6c:rt:t8:t9 0x70:rs:t9:t0 0x70:rt:t9:t0 0x78:rs:t9:t0 0x78:rt:t0:t1 0x80:rt:t0:t1 0x84:rt:t1:t2 0x88:rs:t1:t2 0x88:rt:t1:t2 0x90:rs:t1:t2 0x90:rt:t2:t3 0x94:rt:t2:t3 */
/* @SymbolSize: 160 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_alone_flag;

void shougatu_meet_zyotai_lo_serifu_adj(void)
{
    s32 t7;

    if (shougatu_alone_flag == 0) {
        t7 = get_g_zyotai_s(now_girl) & 0x7F;
        switch (t7) {
        case 0:
        case 1:
            shougatu_meet_now_serifu += 2;
            break;
        case 2:
            shougatu_meet_now_serifu += 1;
            break;
        }
    } else {
        shougatu_meet_now_serifu += 2;
    }
    sub_sub_step_up();
}
