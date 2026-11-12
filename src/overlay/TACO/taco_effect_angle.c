/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 1768 */
/* @PostProc: patch_stack_complete --old 88 --new 96 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x1bc --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x30c --offset2 0x310 */
/* @PostProc: swap_instructions --offset1 0x5ac --offset2 0x5b0 */
/* Address: 0x80159888 */
/* Size: 1768 bytes (442 instructions) */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern s32 srand(s32);
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_effect_angle(s32 arg0)
{
    SVECTOR a;
    SVECTOR b;

    srand(taco_phase_cnt);
    b.vx = 0;
    b.vz = 0;
    a.vz = -20000;
    switch (arg0) {
    case 0:
    case 2:
        b.vy = 1024;
        a.vx = 7000;
        a.vy = -1200;
        taco_actor_alloc(3, 33, taco_zero_vec, a, b, 8, 0);
        a.vy = -200;
        taco_actor_alloc(3, 33, taco_zero_vec, a, b, 8, 0);
        b.vy = 3072;
        a.vx = -7000;
        a.vy = -700;
        if (arg0 == 0) {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 10, 224);
        } else {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 10, 0);
        }
        break;
    case 1:
    case 3:
        b.vy = 3072;
        a.vx = -7000;
        a.vy = -1200;
        if (arg0 == 3) {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 8, 80);
        } else {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 8, 0);
        }
        a.vy = -200;
        if (arg0 == 1) {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 8, 80);
        } else {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 8, 0);
        }
        b.vy = 1024;
        a.vx = 7000;
        a.vy = -700;
        if (arg0 == 3) {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 10, 128);
        } else {
            taco_actor_alloc(3, 33, taco_zero_vec, a, b, 8, 0);
        }
        break;
    }
}
