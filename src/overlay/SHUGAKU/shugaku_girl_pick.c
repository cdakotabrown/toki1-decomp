/* @SymbolSize: 412 */
/* @CompileEnv: IDO52_AS1_FORCE_NARROW_FOLD=3 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x4C --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: patch_insn_reg_field 0x50:rt:s5:s6 0x60:rs:s0:fp 0x60:rt:fp:s0 0x84:rt:s5:s6 0x8c:rt:s6:s5 0x90:rt:s5:s6 0xdc:rs:s6:s5 0xdc:rt:s5:s6 0xf4:rs:s0:fp 0xf4:rt:fp:s0 0x108:rt:t2:t3 0x10c:rs:t2:t3 0x114:rt:t3:t4 0x11c:rt:t3:t4 0x140:rt:t4:t5 0x144:rs:t4:t5 0x144:rt:t4:t5 0x148:rt:t6:t7 0x14c:rs:t6:t7 0x14c:rt:t6:t7 0x154:rs:t4:t5 0x154:rt:t5:t6 0x158:rt:t5:t6 0x160:rs:t6:t7 0x160:rt:t7:t8 0x164:rt:t7:t8 0x16c:rt:t8:a0 0x170:rt:t8:a0 */
/* shugaku_girl_pick - SHUGAKU overlay, Address: 0x80139114, Size: 412 bytes */
#include "types.h"

extern u8  girl_appear[];
extern u8  shugaku_pick_count;
extern u8  shugaku_pick_serifu_no;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s16 now_serifu_shugaku;

extern s32  get_g_zyotai_s(s32 a0);
extern s32  get_h_yuukou(s32 a0);
extern s32  get_h_tokimeki(s32 a0);
extern void date_name_set(s32 a0);
extern void shugaku_serifu_set(void);
extern void sub_sub_step_up(void);

void shugaku_girl_pick(void)
{
    s32 best_i;
    s32 best_a0;
    s16 i;
    s32 v0;
    s32 v1;
    s32 a0v;
    u8  lvu;
    s32 sc1, sc2, sc3, sc4;
    s32 picks;
    u8  bi;
    u8 *base;

    shugaku_pick_count = 0;
    shugaku_pick_serifu_no = 0;
    best_i = 255;
    best_a0 = 4;
    base = girl_appear;

    for (i = 0; i < 10; i++) {
        if (base[i] == 0) {
            continue;
        }
        v0 = get_g_zyotai_s(i);
        v1 = v0 & 0x7F;
        a0v = v1 & 0xFF;
        lvu = v1;
        if ((u32)a0v < (u32)best_a0) {
            goto update;
        }
        if (a0v != best_a0) {
            continue;
        }
        bi = best_i;
        sc1 = get_h_yuukou(i) & 0xFF;
        sc2 = get_h_tokimeki(i) & 0xFF;
        sc3 = get_h_tokimeki(bi) & 0xFF;
        sc4 = get_h_yuukou(bi);
        if ((u32)(sc4 + sc3) >= (u32)(sc2 + sc1)) {
            continue;
        }
    update:
        best_i = i & 0xFF;
        best_a0 = lvu & 0xFF;
    }

    v0 = get_g_zyotai_s(best_i);
    if (best_i != 255 && (u32)(v0 & 0x7F) < 2U) {
        shugaku_pick_serifu_no = (u8)(best_i + 50);
        picks = best_i & 0xFF;
        now_girl = picks;
        date_name_set(picks);
        shugaku_serifu_set();
    } else {
        now_serifu_shugaku = (s16)(now_serifu_shugaku + 4);
        now_sub_sub_step = (u8)(now_sub_sub_step + 30);
        now_girl = 255;
    }
    sub_sub_step_up();
}
