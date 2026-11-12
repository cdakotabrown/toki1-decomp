/* @CompileEnv: IDO52_UGEN_TREG_ORDER=t6,t7,t8,t9,t2,t3,t4,t5 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_branch_operands --offsets 0x88 0x90 */
/* @PostProc: patch_insn_reg_field 0x54:rd:s1:s0 0x60:rd:t7:t8 0x64:rs:t7:t8 0x70:rs:s1:s0 0x94:rd:s0:s1 0x9c:rs:s1:s0 0xa8:rs:s1:s0 0xb4:rs:s0:s1 0xc0:rs:s0:s1 0xc4:rd:t8:t9 0xc8:rd:t9:t0 0xcc:rs:t8:t9 0xcc:rt:t9:t0 0xd8:rs:s1:s0 0xe0:rs:s1:s0 0xe0:rt:s1:s0 0xe8:rs:s1:s0 */
/* Address: 0x8013288C */
/* Size: 424 bytes (106 instructions) */
#include "types.h"

extern void goto_sub_step(s32);
extern s32 get_g_zyotai_s(s32);
extern s32 get_h_tokimeki(s32);
extern s32 get_h_yuukou(s32);
extern void date_name_set(s32);
extern void shougatu_girl_serifu_set(void);

extern u8 screen_w[];
extern u8 day_flag;
extern u8 now_girl;
extern s32 pick_girl;
extern s16 shougatu_meet_now_serifu;
extern s16 shougatu_meet_now_serifu_in;
extern u8 shougatu_girl_found;
extern u8 shougatu_meet_girl;

void shougatu_girl_judge(void)
{
    s32 i;
    u32 best;
    u32 bestVal;
    u32 cur;
    u8 curB;
    u32 prev;
    u32 x0;
    u32 x1;
    u32 x2;
    u32 res;
    u8 *rec;

    pick_girl = 0;
    best = 255;
    bestVal = 4;
    rec = screen_w;
    i = 0;
    do {
        if ((*(s32 *)(rec + 456) << 30) < 0) {
            cur = get_g_zyotai_s(i) & 0x7F;
            curB = (u8)cur;
            if ((cur & 0xFF) < bestVal) {
                goto upd;
            }
            if ((cur & 0xFF) == bestVal) {
                if ((cur & 0xFF) == bestVal) {
                    prev = best ^ 0;
                    x0 = get_h_yuukou(i) & 0xFF;
                    x1 = get_h_tokimeki(i) & 0xFF;
                    x2 = get_h_tokimeki(prev) & 0xFF;
                    if ((u32)get_h_yuukou(prev) + x2 < x1 + x0) {
                        goto upd;
                    }
                }
            }
            goto next;
        upd:
            best = (u32)i & 0xFF;
            bestVal = curB & 0xFF;
        }
    next:
        rec += 56;
        i++;
    } while (i != 11);
    res = get_g_zyotai_s(best);
    if (best != 255) {
        if ((u32)(res & 0x7F) < 2) {
            shougatu_meet_girl = (u8)best;
            now_girl = (u8)(best & 0xFF);
            date_name_set(best & 0xFF);
            shougatu_girl_serifu_set();
            shougatu_meet_now_serifu = 0;
            shougatu_meet_now_serifu_in = 0;
            shougatu_girl_found = 1;
            pick_girl = best + 1;
            day_flag |= 0x80;
            goto_sub_step(1);
            return;
        }
    }
    shougatu_girl_found = 0;
    goto_sub_step(2);
}
