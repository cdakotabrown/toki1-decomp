/* @SymbolSize: 340 */
/* @PostProc: patch_stack_complete --old 64 --new 96 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: remap_stack_slots --from 52 --to 68 --from 56 --to 80 --from 58 --to 82 --from 60 --to 84 */
/* @PostProc: patch_insn_reg_field 0x48:rt:t6:t7 0x4c:rs:t6:t7 0x4c:rt:t6:t7 0x54:rs:t6:t7 0x54:rt:t7:t8 0x5c:rs:t7:t6 0xc4:rd:t8:t7 0xc8:rd:t7:t6 0xc8:rt:t6:t7 0xcc:rs:t7:t6 0xdc:rt:t6:t7 0xf0:rs:t6:t7 0xf0:rt:t6:t7 0x124:rs:t6:t7 0x128:rs:t6:t7 0x138:rs:t6:t7 0x13c:rs:t6:t7 */
/* Address: 0x80156F60 */
/* Size: 340 bytes (85 insns) */
#include "types.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} Vec8;

extern s32 taco_phase_cnt;
extern Vec8 taco_zero_vec;
extern Vec8 taco_emit_rot;
extern s16 *taco_actor_rec;

extern void srand(s32 arg0);
extern s32 rand(void);
extern void taco_actor_alloc(s32 a0, s32 a1, Vec8 a, Vec8 b, Vec8 c,
                          s32 d, s32 e);

void taco_actor_spawn_random(void)
{
    Vec8 c;
    s32 prev;
    s32 cur;

    srand(taco_phase_cnt);

    prev = rand();
    cur = rand();
    c.vx = (cur % 1500) + taco_actor_rec[58] + (prev % 500) - 1000;

    prev = rand();
    cur = rand();
    c.vy = (cur % 1000) + (prev % 500) - 1500;
    c.vz = -32768;

    taco_actor_alloc(11, 16, taco_zero_vec, c, taco_emit_rot, 6, 0);
}
