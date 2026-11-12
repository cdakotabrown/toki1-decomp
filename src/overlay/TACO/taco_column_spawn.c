/* Address: 0x80155394 */
/* Size: 1084 bytes (271 instructions) */
/* @PostProc: patch_stack_complete --old 64 --new 88 */
/* @PostProc: remap_stack_slots --map '48:80,50:82,52:84,62:70,56:64' */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1b8 */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x2f4 --offset2 0x2f8 */
/* @PostProc: swap_instructions --offset1 0x394 --offset2 0x398 */
#include "types.h"

typedef struct { s16 vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern s32 taco_substep_no;
extern SVECTOR taco_zero_vec;
extern u8 *taco_actor_rec;

extern void srand(s32);
extern s32 taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_column_spawn(void)
{
    SVECTOR dir;
    SVECTOR pos;

    srand(taco_phase_cnt);
    pos.vx = *(s16 *)(taco_actor_rec + 116);
    if (*(s16 *)(taco_actor_rec + 116) > 0) {
        pos.vy = 1;
    } else {
        pos.vy = -1;
    }
    pos.vz = -700;
    dir.pad = 0;
    if (taco_substep_no % 3 == 0) {
        taco_actor_alloc(4, 2, pos, taco_zero_vec, dir, 6, 224);
    } else {
        taco_actor_alloc(4, 2, pos, taco_zero_vec, dir, 6, 80);
    }
    dir.pad = 20;
    pos.vz = -900;
    taco_actor_alloc(4, 2, pos, taco_zero_vec, dir, 6, 0);
    dir.pad = 20;
    pos.vz = -500;
    taco_actor_alloc(4, 2, pos, taco_zero_vec, dir, 6, 0);
    dir.pad = 40;
    pos.vz = -1100;
    taco_actor_alloc(4, 2, pos, taco_zero_vec, dir, 6, 0);
    dir.pad = 40;
    pos.vz = -300;
    taco_actor_alloc(4, 2, pos, taco_zero_vec, dir, 6, 0);
    taco_substep_no = taco_substep_no + 1;
}
