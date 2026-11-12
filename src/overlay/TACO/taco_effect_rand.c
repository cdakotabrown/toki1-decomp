/* @RodataBase: 0x8015DF54 */
/* @SymbolSize: 1048 */
/* @PostProc: patch_stack_complete --old 72 --new 96 */
/* @PostProc: remap_stack_slots --from 48 --to 68 --from 56 --to 80 --from 58 --to 82 --from 60 --to 84 --from 64 --to 88 --from 66 --to 90 */
/* Address: 0x80159270 */
/* Size: 1048 bytes (262 insns) */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 w;
} Vec4;

extern s32 taco_phase_cnt;
extern Vec4 taco_emit_rot;

extern s32 rand(void);
extern void srand(s32);
extern void taco_actor_alloc(s32, s32, Vec4, Vec4, Vec4, s32, s32);

void taco_effect_rand(void)
{
    Vec4 vel;
    Vec4 pos;
    u32 sel;

    srand(taco_phase_cnt);

    pos.x = rand() % 1500 + rand() % 2500 - 2000;
    pos.y = rand() % 1000 + rand() % 2000 - 2000;
    pos.z = rand() % 5000 - 25000;
    vel.x = rand() % 200 - 100;
    vel.y = rand() % 200 - 100;

    sel = (u32)rand() % 5;
    switch (sel) {
    case 0:
        taco_actor_alloc(2, 32, vel, pos, taco_emit_rot, 4, 16);
        break;
    case 1:
        taco_actor_alloc(2, 32, vel, pos, taco_emit_rot, 4, 16);
        break;
    case 2:
        taco_actor_alloc(2, 32, vel, pos, taco_emit_rot, 4, 0);
        break;
    case 3:
        taco_actor_alloc(2, 32, vel, pos, taco_emit_rot, 4, 0);
        break;
    case 4:
        taco_actor_alloc(2, 32, vel, pos, taco_emit_rot, 4, 0);
        break;
    }
}
