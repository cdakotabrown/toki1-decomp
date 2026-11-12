/* @PostProc: patch_insn_reg_field 0x1c:rt:t6:t7 0x20:rs:t6:t7 0x20:rt:t6:t7 0x2c:rt:t7:t6 0x5c:rt:t8:t0 0x60:rs:t8:t0 0x60:rt:t8:t0 0x64:rd:t9:t8 0x68:rd:t0:t9 0x68:rt:t9:t8 0x6c:rs:t8:t9 0x134:rt:t7:t8 0x138:rs:t7:t8 */
/* shugaku_pbirth_top_judge -- SHUGAKU overlay, Address: 0x8013243C, Size: 388 bytes */
#include "types.h"

extern s32 p_birthday;
extern u8  now_month;
extern u8  now_day;
extern u8  girl_appear[];

extern s32  sub_sub_step_up(void);
extern s32  get_g_zyotai_s(s32 a0);
extern s32  get_h_yuukou(s32 a0);
extern s32  get_h_tokimeki(s32 a0);
extern void goto_sub_sub_step(s32 a0);

s32 shugaku_pbirth_top_judge(void)
{
    s32 flags;
    s16 i;
    u8 bestIdx;
    u8 bestRank;
    u8 rank;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 r;

    flags = p_birthday;
    bestIdx = 255;
    bestRank = 4;
    if ((flags & 0xF) != now_month ||
        (((u32)flags << 23) >> 27) != now_day) {
        sub_sub_step_up();
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if (girl_appear[i] == 0) {
            continue;
        }
        r = get_g_zyotai_s(i);
        rank = (u8)(r & 0x7F);
        if ((u32)rank < (u32)bestRank) {
            goto upd;
        }
        if ((u32)rank != (u32)bestRank) {
            continue;
        }
        if (bestRank == rank) {
        a = (s32)(u8)get_h_yuukou(i);
        b = (s32)(u8)get_h_tokimeki(i);
        c = (s32)(u8)get_h_tokimeki(bestIdx);
        d = get_h_yuukou(bestIdx);
            if ((u32)(d + c) < (u32)(b + a)) {
                goto upd;
            }
        }
        continue;
    upd:
        bestIdx = (u8)i;
        bestRank = rank;
    }

    r = get_g_zyotai_s(bestIdx);
    if ((u32)(r & 0x7F) < 2U) {
        if (!bestIdx) {
            goto_sub_sub_step(50);
            return 0;
        }
    }
    return sub_sub_step_up();
}
