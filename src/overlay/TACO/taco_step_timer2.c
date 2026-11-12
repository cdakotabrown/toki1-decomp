/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x84 */
/* @PostProc: patch_insn_reg_field 0x3c:rd:s4:s2 0x44:rd:s2:s4 0x68:rs:s2:s4 0x84:rs:s4:s2 0x90:rs:s4:s2 0x94:rt:v1:v0 0x98:rs:s4:s2 0x9c:rt:v1:v0 0xa4:rs:s2:s4 0xa4:rt:v1:v0 0xa8:rt:s4:s2 0x104:rs:s2:s4 0x110:rs:s2:s4 */
/* taco_step_timer2 -- TACO overlay, Address: 0x8014B760, 292 bytes (73 insns) */
#include "types.h"

extern u8 *volatile taco_actor_rec;
extern u8 taco_rest_key_tbl[];
extern u8 taco_rest_mode_tbl[];

extern void node_pose_set();
extern void taco_step_pose_set();

void
taco_step_timer2(u8 *dst, u8 *src, s32 idx, s32 a3, s32 limit)
{
    s32 val;
    s32 v2;
    s32 i;
    s32 bound;
    u8 *p8;
    u8 *pb;

    if ((u32)limit < (u32)*dst) {
        taco_step_pose_set(idx, a3, *src);
        *src = *src + 1;
        *dst = 0;
    }

    if ((u32)*src >= 8U) {
        bound = idx + 3;
        taco_actor_rec[2854] = 0;
        val = taco_actor_rec[2854];
        i = idx;
        *src = (u8)val;
        *dst = (u8)val;
        if (idx < bound) {
            v2 = idx << 1;
            p8 = &taco_rest_key_tbl[v2 << 2];
            pb = &taco_rest_mode_tbl[v2];
            do {
                node_pose_set(i, 1, *(s32 *)(p8 - 8), *(s32 *)(p8 - 4),
                              pb[-2], pb[-1]);
                i += 1;
                p8 += 8;
                pb += 2;
            } while (i != bound);
        }
    } else {
        *dst = *dst + 1;
    }
}
