/* Function: schedule_birthday_add */
/* Address: 0x8006D5D0 */
/* Size: 436 bytes (109 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers_liveness --address 0x8006D5D0 */
#include "common.h"

extern u8 screen_w;
extern u8 back_clear_b;
extern u8 now_month;
extern u8 byouki_kind;

extern s32 birth_day_check_days(s32 a0, s32 a1, void *a2);

void schedule_birthday_add(s32 *arg0, s32 arg1, void *arg2) {
    u8 *s1;
    u8 *s3;
    s32 s0;

    if (birth_day_check_days(0, now_month + arg1, arg2) == 1) {
        s3 = &screen_w;
        s3[((*arg0) << 2) + 1692] = 50;
        s3[((*arg0) << 2) + 1693] = 0;
        (*arg0)++;
    }

    s0 = 1;
    s3 = &screen_w;
    s1 = &back_clear_b;
    do {
        u32 flags;
        flags = *(u32 *)(s1 + 456);

        if ((s32)(flags << 30) < 0) {
            if ((s32)(flags << 25) < 0) {
                if (birth_day_check_days(s0, now_month + arg1, arg2) == 1) {
                    s3[((*arg0) << 2) + 1692] = 50;
                    s3[((*arg0) << 2) + 1693] = (u8)s0;
                    (*arg0)++;
                }
            }
        }
        s0++;
        s1 += 56;
    } while (s0 != 11);

    if ((byouki_kind & 0xF) != 3) {
        if (birth_day_check_days(15, now_month + arg1, arg2) == 1) {
            s3[((*arg0) << 2) + 1692] = 51;
            (*arg0)++;
        }
    }
}
