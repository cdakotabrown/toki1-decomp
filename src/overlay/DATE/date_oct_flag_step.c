/* @PostProc: patch_insn_reg_field 0x80:rd:t2:t3 0x84:rs:t2:t3 0x88:rt:t3:t4 0x8c:rs:t3:t4 0x8c:rt:t3:t4 0x9c:rs:t3:t4 0xa4:rt:t4:t5 0xa8:rs:t4:t5 0xa8:rt:t4:t5 0xb0:rs:t4:t5 0xb4:rt:t5:t6 0xb8:rs:t5:t6 0xb8:rt:t5:t6 0xc0:rs:t5:t6 0xc4:rt:t6:t7 0xc8:rs:t6:t7 0xc8:rt:t6:t7 0xd0:rs:t6:t7 0xd4:rd:t7:t8 0xd8:rt:t7:t8 0xdc:rt:t8:t9 0xe4:rd:t9:t1 0xe4:rt:t8:t9 0xe8:rs:t9:t1 0xf0:rt:t0:t2 0xf4:rt:t2:t4 0xf8:rs:t0:t2 0xf8:rt:t1:t3 0xfc:rt:t1:t3 0x100:rs:t2:t4 0x100:rt:t2:t4 0x104:rt:t6:t9 0x108:rs:t2:t4 0x110:rd:t3:t5 0x114:rt:t3:t5 0x118:rt:t4:t6 0x120:rs:t4:t6 0x120:rt:t5:t7 0x124:rs:t6:t9 0x128:rt:t5:t7 */
/* Address: 0x80140A94, Size: 356 bytes (89 insns) */
#include "types.h"

extern volatile u8 now_girl;
extern volatile u8 now_year;
extern volatile u8 now_month;
extern volatile u8 now_day;
extern u8 screen_w[];

extern s32 get_g_zyotai_s(s32 a0);
extern void sub_sub_step_up(void);

void date_oct_flag_step(void)
{
    s32 v0;
    u8 *rec;
    s32 t;

    v0 = get_g_zyotai_s(now_girl);

    if (now_year == 95 && now_month == 10 && now_day == 15) {
        rec = &screen_w[now_girl * 56];
        if ((*(s32 *)(rec + 456) << 19) < 0) {
            goto action;
        }
    }

    if (now_year == 96 && now_month == 10 && now_day == 13) {
        rec = &screen_w[now_girl * 56];
        if ((*(s32 *)(rec + 456) << 18) >= 0) {
            goto two;
        }
    } else {
        goto two;
    }

action:
    rec[457] = rec[457] & ~0x10;
    t = v0 & 0x7F;
    rec = &screen_w[now_girl * 56];
    rec[457] = rec[457] & ~0x20;
    if (t != 4) {
        sub_sub_step_up();
    } else {
        sub_sub_step_up();
        sub_sub_step_up();
    }
    return;

two:
    sub_sub_step_up();
    sub_sub_step_up();
}
