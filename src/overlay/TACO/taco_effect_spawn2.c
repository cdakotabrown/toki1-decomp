/* @SymbolSize: 396 */
/* @PostProc: patch_stack_complete --old 64 --new 88 */
/* @PostProc: remap_stack_slots --from 56 --to 72 --from 58 --to 74 --from 60 --to 76 --from 64 --to 88 --from 68 --to 92 --from 72 --to 96 */
/* Address: 0x80158DCC */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern SVECTOR taco_emit_rot;
extern s32 srand(s32);
extern s32 rand();
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_effect_spawn2(short a0, short a1, short a2)
{
    SVECTOR v;
    s32 ret;

    srand(taco_phase_cnt);
    v.vx = a0;
    v.vy = a1;
    v.vz = a2;
    ret = rand();
    if ((ret & 1) == 0) {
        taco_actor_alloc(16, 22, taco_zero_vec, v, taco_emit_rot, 8, 96);
    } else {
        taco_actor_alloc(16, 22, taco_zero_vec, v, taco_emit_rot, 8, 112);
    }
}
