/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --map 28:40 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: patch_insn_reg_field 0x30:rs:t7:a1 0x30:rt:a1:t7 0x58:rs:t0:a1 0x58:rt:a1:t0 0x98:rs:t8:a1 0x98:rt:a1:t8 0xc0:rs:t2:a1 0xc0:rt:a1:t2 0x100:rs:t9:a1 0x100:rt:a1:t9 0x130:rs:t5:a1 0x130:rt:a1:t5 0x17c:rs:t8:v0 0x17c:rt:v0:t8 0x18c:rs:t0:a1 0x18c:rt:a1:t0 0x1d0:rs:t6:a0 0x1d0:rt:a0:t6 0x1e0:rs:t7:a1 0x1e0:rt:a1:t7 */
/* @SymbolStart: 0x80159688 */
/* @SymbolSize: 512 */
#include "types.h"

typedef struct {
    u8  pad0[116];
    s16 x;
    s16 y;
    s16 z;
    s16 ang;
    s16 bx;
    s16 by;
    u8  pad1[8];
} Ent;

extern Ent *taco_actor_rec;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void taco_actor_exit(s32);

void taco_actor_homing(s32 idx)
{
    s32 v;
    s32 d;

    v = rcos(taco_actor_rec[idx].ang * 32);
    taco_actor_rec[idx].x = (s16)(taco_actor_rec[idx].x -
                              ((taco_actor_rec[idx].z * 32 / 25000 * v) >> 12));

    v = rsin(taco_actor_rec[idx].ang * 128);
    taco_actor_rec[idx].y = (s16)(taco_actor_rec[idx].y -
                              ((taco_actor_rec[idx].z * 16 / 25000 * v) >> 12));

    if (taco_actor_rec[idx].ang >= 301) {
        taco_actor_rec[idx].z = (s16)(taco_actor_rec[idx].z + 256);
        d = 10;
        taco_actor_rec[idx].x = (s16)(taco_actor_rec[idx].x +
                                  (taco_actor_rec[0].x + taco_actor_rec[idx].bx -
                                   taco_actor_rec[idx].x) / d);
        taco_actor_rec[idx].y = (s16)(taco_actor_rec[idx].y +
                                  (taco_actor_rec[0].y + taco_actor_rec[idx].by -
                                   taco_actor_rec[idx].y) / d);
    }

    if (taco_actor_rec[idx].z >= 4097) {
        taco_actor_exit(idx);
    }
}
