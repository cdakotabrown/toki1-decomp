/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 80 --new 88 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/TACO.EXN --address 0x801573D8 */
/* Address: 0x801573D8 */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern s32 srand(s32);
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_emit_row(void)
{
    SVECTOR v1;
    SVECTOR v2;

    srand(taco_phase_cnt);

    v1.vx = 500;
    v1.vy = 300;
    v1.vz = -32768;
    v2.vx = 0;
    v2.vy = 2048;
    v2.vz = 0;
    v2.pad = 0;
    taco_actor_alloc(8, 18, taco_zero_vec, v1, v2, 4, 16);

    v1.vx = 250;
    v1.vy = 300;
    v1.vz = -30768;
    v2.pad = 1;
    taco_actor_alloc(8, 18, taco_zero_vec, v1, v2, 6, 0);

    v1.vx = 0;
    v1.vy = 300;
    v1.vz = -28768;
    v2.pad = 2;
    taco_actor_alloc(8, 18, taco_zero_vec, v1, v2, 2, 80);

    v1.vx = -250;
    v1.vy = 300;
    v1.vz = -30768;
    v2.pad = 3;
    taco_actor_alloc(8, 18, taco_zero_vec, v1, v2, 6, 0);

    v1.vx = -500;
    v1.vy = 300;
    v1.vz = -32768;
    v2.pad = 4;
    taco_actor_alloc(8, 18, taco_zero_vec, v1, v2, 4, 16);
}
