/* @PostProc: patch_stack_complete --old 88 --new 112 */
/* @PostProc: remap_stack_slots --from 56 --to 76 --from 68 --to 88 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: patch_insn_reg_field 0xf4:rt:t5:t6 0xfc:rt:t5:t6 0x10c:rt:t5:t6 0x114:rt:t5:t6 0x124:rt:t5:t6 0x12c:rt:t5:t6 0x13c:rt:t5:t6 0x144:rt:t5:t6 0x148:rt:t6:t7 0x150:rs:t6:t7 */
/* Address: 0x80147284 */
/* Size: 340 bytes (85 insns) */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} Mtx;

typedef struct {
    s32 flag;
    Mtx mat;
    u8 rest[44];
} Rec;

typedef struct {
    s32 f0;
    Rec *f4;
    s32 f8;
    s32 fc;
} Node16;

extern Node16 taco_obj[];
extern u8 taco_pose_align_fmt[];

extern void printf();

s32 node_pose_set(s32 idx, s32 n, s16 *pA, s16 *pB, u8 f4, u8 f5)
{
    Mtx m;
    Node16 *node;
    s32 i;
    s32 j;

    if (((u32)pA & 3) != 0 || ((u32)pB & 3) != 0) {
        printf(taco_pose_align_fmt, pA, pB);
        return -1;
    }

    if (f4 == 0) {
        pA += n * 3;
    }
    if (f5 == 0) {
        pB += n * 9;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m.m[i][j] = *pB;
            pB++;
        }
        m.t[i] = *pA;
        pA++;
    }

    node = &taco_obj[idx];
    node->f4->mat = m;
    node->f4->flag = 0;
}
