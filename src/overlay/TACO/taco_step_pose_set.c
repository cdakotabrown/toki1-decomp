/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1bc */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1cc --offset2 0x1d0 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x228 */
/* @PostProc: swap_instructions --offset1 0x228 --offset2 0x22c */
/* @PostProc: swap_instructions --offset1 0x22c --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x360 --offset2 0x364 */
/* @PostProc: swap_instructions --offset1 0x364 --offset2 0x368 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x258 --offset2 0x270 */
/* @PostProc: swap_instructions --offset1 0x264 --offset2 0x270 */
/* @PostProc: swap_instructions --offset1 0x268 --offset2 0x270 */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x270 */
/* Address: 0x8014B884 */
/* Size: 972 bytes (243 insns) */
#include "types.h"

typedef struct {
    u8 pad0[1];
    u8 f1;
    u8 pad1[118];
    s16 f120;
    u8 pad2[10];
    u8 f132;
    u8 pad3[3];
} Rec;

extern Rec *taco_actor_rec;
extern s32 taco_step_ang_tbl[];

extern void taco_pose_keyframe_ease();

void taco_step_pose_set(s32 slot, s32 tbl, s32 sel)
{
    s32 i;
    s32 n;
    s32 *q;

    if (sel < 4) {
        i = 0;
        n = slot;
        q = &taco_step_ang_tbl[tbl];
        do {
            taco_pose_keyframe_ease(n, *q, 1, 0, 0);
            i++;
            n++;
            q++;
        } while (i != 3);
    } else if (sel >= 4 && sel < 8) {
        i = 0;
        n = slot;
        q = &taco_step_ang_tbl[tbl];
        do {
            taco_pose_keyframe_ease(n, *q, 2, 0, 0);
            i++;
            n++;
            q++;
        } while (i != 3);
    } else if (sel >= 12 && sel < 22) {
        i = 0;
        n = slot;
        q = &taco_step_ang_tbl[tbl];
        do {
            taco_pose_keyframe_ease(n, *q, 3, 0, 0);
            i++;
            n++;
            q++;
        } while (i != 3);
    } else if (sel >= 24 && sel < 28) {
        i = 0;
        n = slot;
        q = &taco_step_ang_tbl[tbl];
        do {
            taco_pose_keyframe_ease(n, *q, 1, 0, 0);
            i++;
            n++;
            q++;
        } while (i != 3);
    } else if (sel >= 32 && sel < 42) {
        i = 0;
        n = slot;
        q = &taco_step_ang_tbl[tbl];
        do {
            taco_pose_keyframe_ease(n, *q, 4, 0, 0);
            i++;
            n++;
            q++;
        } while (i != 3);
    } else if (sel >= 46 && sel < 50) {
        i = 0;
        n = slot;
        q = &taco_step_ang_tbl[tbl];
        do {
            taco_pose_keyframe_ease(n, *q, 2, 0, 0);
            i++;
            n++;
            q++;
        } while (i != 3);
    }

    if (sel >= 20 && sel < 30) {
        taco_actor_rec[18].f1 = 64;
        taco_actor_rec[18].f120 = 2048;
        taco_actor_rec[19].f1 = 64;
        taco_actor_rec[19].f120 = 2048;
        taco_actor_rec[19].f132 = 20;
        taco_actor_rec[20].f1 = 64;
        taco_actor_rec[20].f120 = 2048;
        taco_actor_rec[20].f132 = 20;
        taco_actor_rec[21].f1 = 64;
        taco_actor_rec[21].f120 = 2048;
        taco_actor_rec[21].f132 = 20;
        taco_actor_rec[23].f1 = 64;
        taco_actor_rec[23].f120 = 2048;
        taco_actor_rec[23].f132 = 20;
        taco_actor_rec[24].f1 = 64;
        taco_actor_rec[24].f120 = 2048;
        taco_actor_rec[24].f132 = 20;
        taco_actor_rec[25].f1 = 64;
        taco_actor_rec[25].f120 = 2048;
        taco_actor_rec[25].f132 = 20;
    } else if (sel == 30) {
        taco_actor_rec[18].f1 = 128;
        taco_actor_rec[19].f1 = 128;
        taco_actor_rec[20].f1 = 128;
        taco_actor_rec[21].f1 = 128;
        taco_actor_rec[23].f1 = 128;
        taco_actor_rec[24].f1 = 128;
        taco_actor_rec[25].f1 = 128;
    }
}
