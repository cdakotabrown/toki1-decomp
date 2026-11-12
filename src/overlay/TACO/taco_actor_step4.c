/* @PostProc: patch_insn_reg_field 0x34:rs:t7:s0 0x34:rt:s0:t7 0x5c:rt:t4:t3 0x64:rt:t2:t1 0x68:rt:t1:t7 0x6c:rd:t3:t2 0x6c:rt:t2:t1 0x70:rd:t3:t2 0x70:rs:t3:t2 0x70:rt:t2:t1 0x74:rd:t3:t2 0x74:rt:t3:t2 0x78:rd:t5:t4 0x78:rs:t4:t3 0x78:rt:t3:t2 0x7c:rt:t5:t4 0x84:rd:t6:t5 0x88:rd:t7:t6 0x88:rt:t6:t5 0x8c:rs:t1:t6 0x9c:rs:t9:s0 0x9c:rt:s0:t9 0xa8:rd:t2:t1 0xac:rd:t2:t1 0xac:rs:t2:t1 0xb0:rd:t2:t1 0xb0:rt:t2:t1 0xb8:rs:t2:t1 0xbc:rt:t4:t3 0xc0:rs:t4:t3 0xc0:rt:t4:t3 0xc4:rt:t1:t5 0xc8:rs:t4:t3 0xcc:rt:t5:t2 0xd0:rt:t3:t9 0xd4:rd:t6:t4 0xd4:rt:t5:t2 0xd8:rd:t6:t4 0xd8:rs:t6:t4 0xd8:rt:t5:t2 0xdc:rd:t6:t4 0xdc:rt:t6:t4 0xe0:rd:t7:t6 0xe0:rs:t1:t5 0xe0:rt:t6:t4 0xe4:rt:t7:t6 0xe8:rt:t2:t1 0xec:rt:t5:t2 0xf0:rd:t8:t7 0xf4:rd:t9:t8 0xf4:rt:t8:t7 0xf8:rs:t3:t8 0x100:rs:t2:t1 0x100:rt:t2:t1 0x108:rs:t2:s0 0x108:rt:s0:t1 0x10c:rt:t4:t3 0x114:rt:t4:t3 0x118:rs:t5:t2 0x118:rt:t5:t2 0x120:rd:t1:t5 0x120:rs:t5:s0 0x120:rt:s0:t2 0x124:rs:t1:t5 0x134:rt:t8:t7 0x144:rt:t6:t4 0x148:rs:t6:t4 0x148:rt:t6:t4 0x150:rd:t7:t6 0x150:rs:t6:s0 0x150:rt:s0:t4 0x154:rs:t7:t6 0x15c:rt:t8:t7 0x160:rs:t8:t7 */
/* Address: 0x80155DF8 */
/* Size: 392 bytes (98 insns) */
#include "types.h"

typedef struct {
    u8  pad0[116];
    s16 x;
    s16 y;
    s16 z;
    s16 ang;
    s16 bx;
    s16 by;
    s16 bz;
    u8  pad1[6];
} Ent;

extern Ent *taco_actor_rec;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void taco_actor_disp_switch(s32, s32);
extern void taco_actor_exit(s32);

void taco_actor_step4(s32 idx)
{
    s32 h;

    taco_actor_rec[idx].x =
        (s16)(((rsin(taco_actor_rec[idx].ang * 60 + 2048) *
                (1000 - taco_actor_rec[idx].ang * 10)) >> 13) +
              taco_actor_rec[idx].bx);

    taco_actor_rec[idx].y =
        (s16)(((rcos(taco_actor_rec[idx].ang * 60 + 2048) *
                (1000 - taco_actor_rec[idx].ang * 10)) >> 13) +
              taco_actor_rec[idx].by);

    taco_actor_rec[idx].z = taco_actor_rec[idx].bz;

    h = taco_actor_rec[idx].ang;
    if (h >= 101) {
        taco_actor_exit(idx);
        h = taco_actor_rec[idx].ang;
    }

    if ((h & 1) == 0) {
        taco_actor_disp_switch(1, idx);
    } else {
        taco_actor_disp_switch(0, idx);
    }
}
