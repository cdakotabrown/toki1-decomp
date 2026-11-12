/* @PostProc: patch_insn_reg_field 0x34:rs:t7:s0 0x34:rt:s0:t7 0x58:rt:t4:t3 0x60:rt:t2:t1 0x64:rt:t1:t7 0x68:rd:t3:t2 0x68:rt:t2:t1 0x6c:rd:t3:t2 0x6c:rs:t3:t2 0x6c:rt:t2:t1 0x70:rd:t3:t2 0x70:rt:t3:t2 0x74:rd:t5:t4 0x74:rs:t4:t3 0x74:rt:t3:t2 0x78:rt:t5:t4 0x80:rd:t6:t5 0x84:rd:t7:t6 0x84:rt:t6:t5 0x88:rs:t1:t6 0x98:rs:t9:s0 0x98:rt:s0:t9 0xa4:rd:t2:t1 0xa8:rd:t2:t1 0xa8:rs:t2:t1 0xb0:rt:t2:t1 0xb4:rt:t4:t3 0xb8:rs:t4:t3 0xb8:rt:t4:t3 0xbc:rt:t1:t5 0xc0:rs:t4:t3 0xc4:rt:t5:t2 0xc8:rt:t3:t9 0xcc:rd:t6:t4 0xcc:rt:t5:t2 0xd0:rd:t6:t4 0xd0:rs:t6:t4 0xd0:rt:t5:t2 0xd4:rd:t6:t4 0xd4:rt:t6:t4 0xd8:rd:t7:t6 0xd8:rs:t1:t5 0xd8:rt:t6:t4 0xdc:rt:t7:t6 0xe0:rt:t2:t1 0xe4:rt:t5:t2 0xe8:rd:t8:t7 0xec:rd:t9:t8 0xec:rt:t8:t7 0xf0:rs:t3:t8 0xf8:rs:t2:t1 0xf8:rt:t2:t1 0x100:rs:t2:s0 0x100:rt:s0:t1 0x104:rt:t4:t3 0x10c:rt:t4:t3 0x110:rs:t5:t2 0x110:rt:t5:t2 0x118:rd:t1:t5 0x118:rs:t5:s0 0x118:rt:s0:t2 0x11c:rs:t1:t5 0x11c:rt:t6:t4 0x124:rs:t6:t4 0x124:rt:t7:t6 0x128:rs:t7:t6 0x14c:rt:t8:t7 0x150:rs:t8:t7 0x150:rt:t8:t7 0x158:rs:t8:s0 0x158:rt:s0:t7 0x15c:rt:t3:t8 0x164:rs:t3:t8 0x174:rt:t2:t1 0x17c:rs:t2:t1 0x17c:rt:t2:t1 0x180:rt:t5:t2 0x184:rd:t4:t3 0x184:rs:t2:t1 0x188:rs:t4:t3 0x18c:rs:t5:t2 0x18c:rt:t5:t2 0x190:rt:t6:t4 0x194:rd:t1:t5 0x194:rs:t5:t2 0x198:rs:t1:t5 0x19c:rs:t6:t4 0x19c:rt:t6:t4 0x1a4:rd:t7:t6 0x1a4:rs:t6:t4 0x1a8:rs:t7:t6 */
/* Address: 0x80155C4C */
/* Size: 428 bytes (107 insns) */
#include "types.h"

typedef struct {
    u8  pad0[2];
    u8  f2;
    u8  pad1[113];
    s16 x;
    s16 y;
    s16 z;
    s16 ang;
    s16 bx;
    s16 by;
    s16 bz;
    u8  pad2[2];
    u8  f132;
    u8  pad3[3];
} Ent;

extern Ent *taco_actor_rec;
extern u8 taco_kind_rad_tbl[];

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void taco_actor_disp_switch(s32, s32);

void taco_actor_step3(s32 idx)
{
    taco_actor_rec[idx].x =
        (s16)(((rsin(taco_actor_rec[idx].ang * 60) *
                (1000 - taco_actor_rec[idx].ang * 10)) >> 13) +
              taco_actor_rec[idx].bx);

    taco_actor_rec[idx].y =
        (s16)(((rcos(taco_actor_rec[idx].ang * 60) *
                (1000 - taco_actor_rec[idx].ang * 10)) >> 13) +
              taco_actor_rec[idx].by);

    taco_actor_rec[idx].z = taco_actor_rec[idx].bz;

    if ((taco_actor_rec[idx].ang & 1) == 0) {
        taco_actor_disp_switch(0, idx);
    } else {
        taco_actor_disp_switch(1, idx);
    }

    if (taco_actor_rec[idx].ang >= 101) {
        taco_actor_rec[idx].f132 = taco_kind_rad_tbl[5];
        taco_actor_rec[idx].f2 = 5;
        taco_actor_rec[idx].ang = 0;
        taco_actor_rec[idx].by = 0;
    }
}
