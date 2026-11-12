/* @SymbolStart: 0x8015368C */
/* @SymbolSize: 184 */
/* @PostProc: scheduler_normalizer --block-aware --const-fp --store-fp --branch-fp --load-fp */
/* @PostProc: patch_insn_reg_field 0x48:rt:t0:t1 0x4c:rt:v0:t9 0x54:rs:v0:t9 0x54:rt:t9:t0 0x58:rt:t9:t0 0x5c:rs:t0:t1 0x5c:rt:t0:t1 0x64:rs:t0:t1 0x68:rt:t1:t2 0x6c:rs:t1:t2 0x6c:rt:t1:t2 0x74:rs:t1:t2 0x74:rt:t2:t3 0x78:rt:t3:t7 0x7c:rs:t2:t3 0x88:rs:t1:t2 0x8c:rs:t3:t4 0x8c:rt:t3:t4 0x94:rs:t3:t4 0x94:rt:v0:t5 0x9c:rs:t3:t4 0x9c:rt:v0:t5 0xa0:rs:t4:t7 0xa0:rt:t4:t7 0xa8:rs:t4:t7 0xa8:rt:v0:t6 0xb0:rs:v0:t6 0xb0:rt:t5:t9 0xb4:rs:t4:t7 0xb4:rt:t5:t9 */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_frame_pos_set(s32 a0);

void taco_panel_frame_step(s32 a0)
{
    u8 *rec = taco_actor_rec;

    u8 v;

    if ((u32)a0 < (u32)rec[3131]) {
        taco_frame_pos_set(rec[3130]);
        v = taco_actor_rec[3130];
        taco_actor_rec[3130] = v + 1;
        taco_actor_rec[3131] = 0;
    }

    if ((u32)taco_actor_rec[3130] >= 124U) {
        taco_actor_rec[3130] = 0;
        v = taco_actor_rec[3130];
        taco_actor_rec[3131] = v;
    } else {
        v = taco_actor_rec[3131];
        taco_actor_rec[3131] = v + 1;
    }
}
