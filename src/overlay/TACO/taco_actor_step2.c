/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --map 28:40,36:44 */
/* @PostProc: patch_insn_reg_field 0x30:rs:t7:t6 0x30:rt:t6:t7 0x58:rd:t1:at 0x5c:rd:t2:t1 0x5c:rt:t1:at 0x60:rd:t2:t1 0x60:rs:t2:t1 0x60:rt:t1:at 0x64:rd:t2:t1 0x64:rt:t2:t1 0x68:rd:t2:t1 0x68:rs:t2:t1 0x68:rt:t1:at 0x6c:rd:t2:t1 0x6c:rt:t2:t1 0x70:rd:t2:t1 0x70:rs:t2:t1 0x70:rt:t1:at 0x74:rd:t2:t1 0x74:rt:t2:t1 0x78:rd:t2:t1 0x78:rs:t2:t1 0x78:rt:t1:at 0x7c:rd:t2:t1 0x7c:rt:t2:t1 0x80:rt:t4:t3 0x84:rs:t4:t3 0x84:rt:t4:t3 0x8c:rd:t2:t1 0x8c:rs:t2:t1 0x8c:rt:t1:at 0x90:rd:t2:t1 0x90:rt:t2:t1 0x94:rd:t3:t2 0x94:rt:t2:t1 0x9c:rd:t5:t4 0x9c:rs:t4:t3 0xa4:rs:t5:t4 0xa4:rt:t3:t2 0xa8:rt:t6:t5 0xac:rs:t6:t5 0xac:rt:t6:t5 0xb4:rd:t7:t6 0xb8:rs:t6:t5 0xbc:rt:t9:t8 0xc0:rd:t8:t7 0xc0:rt:t7:t6 0xc4:rs:t8:t7 0xc4:rt:t9:t8 0xc8:rt:t1:t0 0xd0:rt:t4:t3 0xd4:rt:t5:t4 0xd8:rt:t7:t6 0xe0:rd:t0:t9 0xe4:rd:t2:t1 0xe4:rs:t1:t9 0xe8:rt:t2:t1 0xec:rs:t4:t3 0xec:rt:t4:t3 0xf0:rt:t2:t1 0xf4:rs:t4:a1 0xf4:rt:a1:t3 0xf8:rt:t3:t2 0xfc:rt:t1:t9 0x100:rt:t3:t2 0x104:rs:t5:t4 0x104:rt:t5:t4 0x108:rd:t4:t3 0x108:rs:t2:t1 0x10c:rd:t6:t5 0x10c:rs:t5:t4 0x110:rs:t6:t5 0x114:rs:t7:t6 0x114:rt:t7:t6 0x118:rt:t5:t4 0x11c:rs:t7:a1 0x11c:rt:a1:t6 0x120:rt:t8:t7 0x128:rs:t8:t7 0x134:rs:t1:t9 0x134:rt:t1:t9 0x138:rt:t9:t8 0x13c:rs:t1:t9 0x144:rt:t9:t8 0x148:rt:t4:t3 0x14c:rs:t5:t4 0x14c:rt:t5:t4 0x150:rt:t3:t2 0x154:rd:t6:t5 0x154:rs:t5:t4 0x158:rs:t6:t5 0x158:rt:t3:t2 */
/* @SymbolStart: 0x801557D0 */
/* @SymbolSize: 372 */
#include "types.h"

typedef struct {
    u8  pad0[100];
    s16 f100;
    s16 f102;
    s16 f104;
    s16 f106;
    u8  pad1[8];
    s16 x;
    s16 y;
    s16 z;
    s16 ang;
    s16 bx;
    s16 by;
    s16 bz;
    u8  pad2[6];
} Ent;

extern Ent *taco_actor_rec;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void taco_actor_exit(s32);

void taco_actor_step2(s32 idx)
{
    s32 a;

    a = (taco_actor_rec[idx].ang - taco_actor_rec[idx].f106) * 7;

    taco_actor_rec[idx].z = (s16)((-rsin(a) * 15000) >> 12);

    taco_actor_rec[idx].x =
        (s16)(((rcos(a) << 11) >> 12) * taco_actor_rec[idx].by +
              taco_actor_rec[idx].bx);
    taco_actor_rec[idx].y = taco_actor_rec[idx].bz;
    taco_actor_rec[idx].f100 = 0;

    if (taco_actor_rec[idx].by == 1) {
        taco_actor_rec[idx].f102 = (s16)a;
        taco_actor_rec[idx].f104 = 3072;
    } else {
        taco_actor_rec[idx].f102 = (s16)(4096 - a);
        taco_actor_rec[idx].f104 = 1024;
    }

    if (a >= 2561) {
        taco_actor_exit(idx);
    }
}
