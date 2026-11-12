/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --map 24:44,28:48,32:52 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1bc */
/* @PostProc: swap_instructions --offset1 0x1bc --offset2 0x1c0 */
/* @PostProc: patch_insn_reg_field 0x104:rt:t3:t4 0x108:rs:t3:t4 0x108:rt:t3:t4 0x10c:rt:t5:t3 0x110:rd:t4:t5 0x110:rt:t3:t4 0x114:rd:t4:t5 0x114:rs:t4:t5 0x114:rt:t3:t4 0x118:rd:t4:t5 0x118:rt:t4:t5 0x11c:rd:t4:t5 0x11c:rs:t4:t5 0x11c:rt:t3:t4 0x120:rd:t4:t5 0x120:rt:t4:t5 0x124:rd:t4:t5 0x124:rs:t4:t5 0x124:rt:t3:t4 0x128:rd:t4:t5 0x128:rt:t4:t5 0x12c:rs:t5:t3 0x12c:rt:t5:t3 0x130:rd:t4:t5 0x130:rs:t4:t5 0x130:rt:t3:t4 0x134:rd:t4:t5 0x134:rt:t4:t5 0x148:rs:t4:t3 */
/* @SymbolStart: 0x8013F160 */
/* @SymbolSize: 480 */
#include "types.h"

extern s16 taco_time_left;
extern u8 taco_time_disp_str0[];
extern u8 taco_time_disp_str1[];
extern u8 taco_round_no;
extern u8 taco_bonus_timer;
extern u8 *taco_bonus_score;
extern u8 *taco_score;

extern void taco_str_disp2(s32, s32, s32, s32);
extern void taco_num_disp(s32, s32, s32, s32);
extern void taco_hud_disp(void);
extern void xa_rand_set(void);

void taco_time_disp(void)
{
    s32 t;
    s32 hi;
    s32 lo;
    s32 h;
    s32 m;
    s32 s;
    s32 v;

    t = taco_time_left;
    hi = 3600;
    lo = 60;
    h = t / hi;
    m = (t % hi) / lo;
    s = ((t % lo) * lo) / 100;

    taco_str_disp2((s32)taco_time_disp_str0, -224, -100, 0);
    taco_num_disp((s32)(taco_score + taco_round_no * 1000000), -80, -100, 0);
    taco_str_disp2((s32)taco_time_disp_str1, 64, -100, 0);
    taco_num_disp(h, 144, -100, 1);
    taco_num_disp(m, 176, -100, 2);
    taco_num_disp(s, 208, -100, 3);
    taco_hud_disp();

    v = taco_bonus_timer;
    if (v != 0) {
        taco_bonus_timer = (u8)(v - 1);
        taco_num_disp((s32)taco_bonus_score, 20, 100, 3);
    }
    xa_rand_set();
}
