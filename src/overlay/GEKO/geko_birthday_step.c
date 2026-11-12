/* @PostProc: patch_insn_reg_field 0x5c:rs:t6:fp 0x5c:rt:fp:t6 0x68:rd:t9:t0 0x6c:rs:t9:t0 0xc4:rd:t0:t1 0xc4:rs:s4:v0 0xc4:rt:v0:s4 0xc8:rd:t1:t2 0xcc:rs:t0:t1 0xcc:rt:t1:t2 0xe4:rd:t2:t3 0xe8:rt:t2:t3 0x110:rt:t6:t5 0x114:rs:t6:t5 0x114:rt:t6:t5 0x118:rt:t5:t6 0x134:rt:a1:v1 0x138:rs:a1:v1 0x13c:rt:t1:t2 0x148:rs:t1:t2 0x150:rt:t2:t3 0x154:rs:t2:t3 0x154:rt:t2:t3 0x158:rt:t3:t8 0x15c:rs:t2:s7 0x15c:rt:s7:t3 0x16c:rt:t3:t8 */
/* geko_birthday_step -- GEKO overlay, Address: 0x80135888, Size: 416 bytes (104 insns) */
#include "types.h"

extern s32  get_g_zyotai_s(s32 a0);
extern s32  get_h_yuukou(s32 a0);
extern s32  get_h_tokimeki(s32 a0);
extern s32  goto_sub_sub_step(s32 a0);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern s32 p_birthday;
extern u8  now_month;
extern u8  now_day;
extern u8  day_flag;
extern u8  screen_w[];

s32 geko_birthday_step(void)
{
    s16 i;
    u8  best;
    u8  bestScore;
    u8  score;
    u8  a;
    u8  cand;
    u8  c1;
    u8  c2;
    u8  c3;
    u8  key;
    s32 c4;
    s32 v0;
    s32 date;
    u8 *base;

    base = (u8 *)((u32)screen_w | 0);
    best = 255;
    bestScore = 4;

    for (i = 0; i < 11; i++) {
        if ((*(s32 *)&base[i * 56 + 456] << 30) < 0) {
            v0 = get_g_zyotai_s(i);
            a = (u8)(v0 & 0x7F);
            score = (u8)(v0 & 0x7F);
            if (!((u32)a < (u32)bestScore)) {
                if (bestScore != a) {
                    continue;
                }
                cand = best;
                c1 = (u8)get_h_yuukou(i);
                c2 = (u8)get_h_tokimeki(i);
                c3 = (u8)get_h_tokimeki(cand);
                c4 = get_h_yuukou(cand);
                if (!((u32)(c4 + c3) < (u32)(c2 + c1))) {
                    continue;
                }
            }
            best = (u8)i;
            bestScore = score;
        }
    }

    v0 = get_g_zyotai_s(now_girl);

    date = p_birthday;
    if (now_month != (u8)(date & 0x0F)) {
        goto next;
    }
    if ((u32)(date << 23) >> 27 != now_day) {
        goto next;
    }
    key = day_flag;
    if ((key & 0x04) != 0) {
        goto next;
    }
    if ((u32)(v0 & 0x7F) >= 2U) {
        goto next;
    }
    if (best != now_girl) {
        goto next;
    }
    day_flag = (u8)(key | 0x44);
    goto_sub_sub_step(52);
    return 0;

next:
    sub_sub_step_up();
}
