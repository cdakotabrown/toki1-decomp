/* @PostProc: patch_stack_complete --old 56 --new 88 */
/* @PostProc: remap_stack_slots --from 48 --to 72 --from 50 --to 74 --from 52 --to 76 --from 56 --to 88 --from 60 --to 92 --from 64 --to 96 */
/* Address: 0x80158CEC */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern SVECTOR taco_emit_rot;
extern s32 srand(s32);
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_effect_spawn(short a0, short a1, short a2)
{
    SVECTOR v;

    srand(taco_phase_cnt);
    v.vx = a0;
    v.vy = a1;
    v.vz = a2;
    taco_actor_alloc(15, 22, taco_zero_vec, v, taco_emit_rot, 4, 208);
}
