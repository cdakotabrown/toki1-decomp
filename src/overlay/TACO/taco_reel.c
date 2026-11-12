/* @PostProc: patch_stack_complete --old 56 --new 136 */
/* @PostProc: patch_insn_reg_field 0x138:rt:t2:t3 0x14c:rt:t1:t2 0x154:rs:t1:t2 0x16c:rs:t2:t3 0x16c:rt:t3:t4 0x174:rs:t3:t4 0x174:rt:t4:t5 0x178:rs:t2:t3 0x178:rt:t4:t5 */
/* @PostProc: remap_stack_slots --map '48:64,52:72' */
/* Address: 0x80153038 */
/* Size: 384 bytes (96 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 rand(void);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern void taco_rows_disp(s32, s32);
extern void taco_reel_glyph_disp(s32, s32, s32);
extern void taco_reel_result_set(s32);

void taco_reel(u8 *timer, u8 *step, s32 kind, s32 arg3, s32 arg4)
{
    s32 idx;
    s32 x;
    s32 y;

    if ((u32)arg4 < (u32)*timer) {
        taco_reel_glyph_disp(kind, arg3, *step);
        *step = *step + 1;
        *timer = 0;
        if (kind == 1) {
            idx = 16;
        } else if (kind == 2) {
            idx = 17;
        } else if (kind == 3) {
            idx = 18;
        }
        if (taco_actor_rec[idx * 136 + 133] != 0) {
            if ((*step & 1) == 0) {
                x = rand();
                y = rand();
                taco_node_shot_spawn(0, kind, (s16)((x & 0x1FF) - 256),
                              (s16)((y & 0x1FF) + 244), 1000, 400, 130);
            }
        }
    }

    if ((u32)*step >= 20U) {
        *step = 0;
        *timer = 0;
        taco_reel_result_set(kind);
        taco_rows_disp(kind, arg3);
    } else {
        *timer = *timer + 1;
    }
}
