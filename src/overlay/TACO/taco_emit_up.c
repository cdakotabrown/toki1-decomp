/* @SymbolSize: 212 */
/* @PostProc: patch_stack_complete --old 56 --new 96 */
/* @PostProc: remap_stack_slots --from 48 --to 72 --from 50 --to 74 --from 52 --to 76 --from 54 --to 78 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/TACO.EXN --address 0x80158AC0 */
/* Address: 0x80158AC0 */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern SVECTOR taco_emit_pos;
extern s32 srand(s32);
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_emit_up(void)
{
    SVECTOR v;

    srand(taco_phase_cnt);
    v.vx = 0;
    v.vy = 1024;
    v.vz = 0;
    v.pad = 0;
    taco_actor_alloc(14, 21, taco_zero_vec, taco_emit_pos, v, 16, 224);
}
