/* Address: 0x80155944 */
/* Size: 776 bytes (194 instructions) */
/* @PostProc: patch_stack_complete --old 88 --new 96 */
/* @PostProc: remap_stack_slots --map '72:88,74:90,76:92,80:72,82:74,84:76,60:64,68:64' */
/* @PostProc: patch_insn_reg_field 0x48:rt:t6:t7 0x4c:rs:t6:t7 0x4c:rt:t6:t7 0x54:rs:t6:t7 0x54:rt:t7:t8 0x58:rd:t8:t6 0x5c:rs:t7:t6 0x94:rt:t4:t5 0x98:rs:t4:t5 0x98:rt:t4:t5 0xa0:rs:t4:t5 0xa0:rt:t5:t7 0xa4:rt:t4:t5 0xa8:rs:t4:t5 0xa8:rt:t4:t5 0xcc:rd:t6:t4 0xd0:rd:t7:t6 0xd0:rs:t5:t4 0xd0:rt:t6:t7 0xd4:rt:t6:t7 0xe8:rs:t7:t6 0xf0:rs:t4:t5 0x100:rd:t5:t4 0x104:rs:t5:t4 0x10c:rt:t6:t7 0x110:rs:t6:t7 0x114:rs:t6:t7 0x124:rs:t6:t7 0x124:rt:t7:t6 0x128:rs:t6:t7 0x128:rt:t7:t6 0x130:rt:t7:t6 0x134:rt:t7:t6 0x168:rt:t5:t4 0x174:rt:t4:t5 0x178:rt:t4:t5 0x180:rt:t4:t5 0x184:rt:t5:t4 0x194:rt:t4:t5 0x1a0:rs:t6:t7 0x1a4:rs:t6:t7 0x1b4:rs:t6:t7 0x1b8:rs:t6:t7 0x1f8:rt:t7:t6 0x204:rt:t5:t4 0x208:rt:t5:t4 0x20c:rt:t7:t6 0x210:rt:t5:t4 0x21c:rt:t5:t4 0x22c:rt:t6:t7 0x234:rs:t6:t7 0x234:rt:t6:t7 0x240:rs:t6:t7 0x260:rt:t4:t5 0x264:rt:t4:t5 0x26c:rt:t4:t5 0x270:rt:t4:t5 0x27c:rt:t7:t6 0x29c:rs:t7:t6 0x2a0:rs:t7:t6 0x2b0:rs:t7:t6 0x2b4:rs:t7:t6 0x2dc:rt:t5:t4 0x2f4:rd:t4:t5 0x2f8:rs:t4:t5 0x2fc:rs:t5:t4 0x2fc:rt:t5:t4 0x308:rs:t5:t4 */
#include "types.h"

typedef struct { s16 vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern s32 taco_substep_no;
extern SVECTOR taco_zero_vec;
extern u8 *taco_actor_rec;

extern void srand(s32);
extern s32 rand(void);
extern s32 taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_scatter_spawn(void)
{
    SVECTOR dir;
    SVECTOR pos;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 h;

    srand(taco_phase_cnt);
    r1 = rand();
    r2 = rand();
    pos.vx = r2 % 250 + *(s16 *)(taco_actor_rec + 116) + r1 % 150 - 200;
    r3 = rand();
    r4 = rand();
    pos.vy = r4 % 135 + *(s16 *)(taco_actor_rec + 118) + r3 % 75 - 100;
    pos.vz = -7500;
    dir.vy = 2048;
    dir.vz = 0;
    dir.vx = 0;
    if (taco_substep_no % 3 == 0) {
        h = taco_actor_alloc(5, 3, pos, taco_zero_vec, dir, 4, 128);
    } else {
        h = taco_actor_alloc(5, 3, pos, taco_zero_vec, dir, 4, 16);
    }
    if (h != -1) {
        *(taco_actor_rec + h * 136 + 132) = 0;
    }
    h = taco_actor_alloc(5, 4, pos, taco_zero_vec, dir, 2, 0);
    if (h != -1) {
        *(taco_actor_rec + h * 136 + 132) = 0;
    }
    taco_substep_no = taco_substep_no + 1;
}
