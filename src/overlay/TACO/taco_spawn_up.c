/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x12c */
/* @PostProc: swap_instructions --offset1 0x12c --offset2 0x130 */
/* @PostProc: swap_instructions --offset1 0x130 --offset2 0x134 */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x14c */
/* @PostProc: patch_insn_reg_field 0xdc:rd:v0:v1 0xe8:rd:v0:v1 0xe8:rs:v0:v1 0xec:rd:v0:v1 0xec:rt:v0:v1 0xf0:rd:v1:a1 0xf0:rs:t6:v1 0xf0:rt:v0:t6 0xf4:rs:v1:a1 0xf4:rt:a1:v0 0xfc:rs:a1:v0 0x108:rs:a1:v0 0x110:rs:a1:v0 0x118:rs:a1:v0 0x11c:rt:t4:t0 0x128:rs:v1:a1 0x134:rt:v0:v1 0x144:rt:v0:v1 0x158:rt:v0:v1 0x16c:rd:a0:a1 0x16c:rt:v0:v1 0x170:rs:a0:a1 0x170:rt:a1:v0 0x178:rs:a0:a1 0x178:rt:a1:v0 0x184:rt:v0:v1 0x188:rt:a1:v0 0x194:rt:v0:v1 0x1a4:rd:a0:a1 0x1a4:rs:t0:v1 0x1a4:rt:v0:t0 0x1a8:rs:a0:a1 0x1b8:rs:a0:a1 0x1bc:rs:v1:a1 0x1bc:rt:a1:v0 0x1c0:rt:t9:t7 0x1c4:rt:a1:v0 0x1c8:rt:a1:v0 0x1d0:rd:a1:v0 0x1d4:rs:v1:a1 0x1d4:rt:a1:v0 0x1d8:rs:t9:t7 0x1d8:rt:t9:t7 0x1dc:rt:t0:t9 0x1e0:rd:t1:t8 0x1e0:rs:t9:t7 0x1e0:rt:v0:v1 0x1e4:rs:t1:t8 0x1e4:rt:a1:v0 0x1e8:rs:t0:t9 0x1e8:rt:t0:t9 0x1ec:rt:t4:t0 0x1f0:rd:t2:t1 0x1f0:rs:t0:t9 0x1f0:rt:v0:v1 0x1f4:rs:t2:t1 0x1f4:rt:a1:v0 0x1f8:rs:t4:t0 0x1f8:rt:t4:t0 0x200:rd:a0:a1 0x200:rs:t4:v1 0x200:rt:v0:t0 0x204:rs:a0:a1 0x204:rt:t3:t2 0x20c:rs:t3:t2 0x218:rs:a0:a1 0x218:rt:t5:t4 0x220:rs:t5:t4 0x220:rt:t6:t3 0x228:rs:a0:a1 0x228:rt:t6:t3 0x22c:rs:v1:a1 0x22c:rt:t7:t5 0x230:rt:t9:t7 0x234:rs:t7:t5 0x234:rt:t8:t6 0x238:rs:v1:a1 0x238:rt:t8:t6 0x23c:rs:t9:t7 0x23c:rt:t9:t7 0x244:rd:a0:a1 0x244:rs:t9:v1 0x244:rt:v0:t7 0x248:rs:a0:a1 0x248:rt:t1:t8 0x250:rs:t1:t8 0x25c:rs:a0:a1 0x25c:rt:t0:t9 0x264:rs:t0:t9 0x264:rt:t2:t1 0x26c:rs:a0:a1 0x26c:rt:t2:t1 0x270:rs:v1:a1 0x270:rt:t4:t0 */
/* Address: 0x80156160 */
/* Size: 620 bytes (155 insns) -- .sym merges TWO functions: */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern volatile s32 taco_actor_rec;
extern s32 srand(s32);
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_spawn_up(void)
{
    SVECTOR v1;

    srand(taco_phase_cnt);

    v1.vx = -256;
    v1.vy = 2048;
    v1.vz = 0;
    taco_actor_alloc(6, 6, taco_zero_vec, taco_zero_vec, v1, 255, 240);
}

void taco_actor_step6(s32 a0)
{
    s32 elem;
    s32 a1;
    s32 p;
    s16 v0;

    elem = a0 * 136;
    a1 = (s32)taco_actor_rec + elem;
    switch (*(s16 *)(a1 + 0x7c)) {
    case 0:
        *(s16 *)(a1 + 0x78) = -32768;
        *(s16 *)((s32)taco_actor_rec + elem + 0x74) = 0;
        *(s16 *)((s32)taco_actor_rec + elem + 0x76) = -500;
        *(s16 *)((s32)taco_actor_rec + elem + 0x70) = 1;
        p = (s32)taco_actor_rec + elem;
        v0 = *(s16 *)(p + 0x70);
        *(s16 *)(p + 0x6e) = v0;
        *(s16 *)((s32)taco_actor_rec + elem + 0x6c) = v0;
        *(s8 *)((s32)taco_actor_rec + elem + 0x84) = 0;
        p = (s32)taco_actor_rec + elem;
        *(s16 *)(p + 0x7c) = *(s16 *)(p + 0x7c) + 1;
        break;
    case 1:
        v0 = ((*(s16 *)(a1 + 0x7a) * 5) << 18) >> 16;
        *(s16 *)(a1 + 0x70) = v0;
        *(s16 *)((s32)taco_actor_rec + elem + 0x6e) = v0;
        *(s16 *)((s32)taco_actor_rec + elem + 0x6c) = v0;
        p = (s32)taco_actor_rec + elem;
        if (*(s16 *)(p + 0x7a) < 501) {
            return;
        }
        *(s16 *)(p + 0x7c) = *(s16 *)(p + 0x7c) + 1;
        break;
    case 2:
        *(s16 *)(a1 + 0x78) = *(s16 *)(a1 + 0x78) + 50;
        p = (s32)taco_actor_rec + elem;
        if (*(s16 *)(p + 0x78) < -24999) {
            return;
        }
        *(s16 *)(p + 0x7c) = *(s16 *)(p + 0x7c) + 1;
        break;
    case 3:
        *(s8 *)(a1 + 0x84) = 32;
        break;
    }
}
