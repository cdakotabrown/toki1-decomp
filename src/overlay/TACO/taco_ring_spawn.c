/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 128 --new 136 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/TACO.EXN --address 0x8015A174 */
/* Address: 0x8015A174 */
#include "types.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern SVECTOR taco_zero_vec;
extern SVECTOR taco_emit_rot;
extern s32 srand(s32);
extern s32 rand();
extern void taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_ring_spawn(s32 count)
{
    SVECTOR sv6;
    SVECTOR sv3;
    s32 i;
    s32 acc;
    s32 ret2;

    srand(taco_phase_cnt);
    sv6.vz = 20000;
    taco_actor_alloc(13, 35, taco_zero_vec, sv6, taco_emit_rot, 40, 208);
    if (count > 0) {
        i = 0;
        acc = 0;
        do {
            sv3.vx = 0;
            sv3.vy = 2048;
            sv3.vz = 0;
            sv3.pad = acc / count;
            ret2 = rand();
            taco_actor_alloc(13, 34, taco_zero_vec, sv6, sv3, 20, (ret2 % 9) << 4);
            i++;
            acc += 4096;
        } while (i != count);
    }
}
