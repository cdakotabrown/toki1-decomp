/* @SymbolStart: 0x801572E8 */
/* @SymbolSize: 240 */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: remap_stack_slots --from 52 --to 64 --from 54 --to 66 --from 56 --to 68 --from 58 --to 70 --from 60 --to 72 --from 62 --to 74 --from 64 --to 76 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/TACO.EXN --address 0x801572E8 */
/* Address: 0x801572F0 */
#include "types.h"

extern s32 taco_phase_cnt;

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern SVECTOR taco_zero_vec;
extern s32 srand(s32);
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_emit_neg_x(void)
{
    s32 arg = taco_phase_cnt;

    SVECTOR v1;
    SVECTOR v2;

    srand(arg + 0x1E240);
    v1.vx = -1000;
    v1.vy = 300;
    v1.vz = -32768;
    v2.vx = 0;
    v2.vy = 2048;
    v2.vz = 0;
    v2.pad = 1;
    taco_actor_alloc(8, 18, taco_zero_vec, v1, v2, 4, 0);
}
