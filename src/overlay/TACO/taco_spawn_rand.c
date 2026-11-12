/* @SymbolStart: 0x80154F5C */
/* @SymbolSize: 700 */
/* Address: 0x80154F64 */
/* Size: 692 bytes (173 instructions) */
/* either address exists in the lowercase-hex tail section.  @SymbolSize: 692 */
/* @PostProc: patch_stack_complete --old 88 --new 96 */
/* @PostProc: remap_stack_slots --map '72:80,74:82,76:84,80:72,82:74,84:76,86:78,68:64,60:64' */
/* @PostProc: patch_insn_reg_field 0x50:rt:t6:t7 0x54:rs:t6:t7 0x54:rt:t6:t7 0x5c:rs:t6:t7 0x5c:rt:t7:t8 0x60:rd:t8:t6 0x64:rs:t7:t6 0x9c:rt:t4:t5 0xa0:rs:t4:t5 0xa0:rt:t4:t5 0xa8:rs:t4:t5 0xa8:rt:t5:t7 0xb0:rd:t6:t4 0xb4:rd:t7:t6 0xb4:rs:t5:t4 0xb4:rt:t6:t7 0xc4:rs:t7:t6 0xf4:rt:t4:t5 0xf8:rs:t4:t5 0x100:rs:t4:t5 0x100:rt:t4:t5 0x108:rt:t4:t5 0x110:rt:t5:t4 0x114:rs:t5:t4 0x11c:rs:t5:t4 0x11c:rt:t5:t4 0x124:rt:t5:t4 0x130:rd:t6:t7 0x134:rt:t6:t7 0x174:rd:t7:t6 0x178:rs:t7:t6 0x1a0:rt:t6:t7 0x1b4:rt:t6:t7 0x1c8:rt:t7:t6 0x1e8:rt:t5:t4 0x1ec:rt:t5:t4 0x1f4:rt:t5:t4 0x1fc:rt:t7:t6 0x20c:rt:t5:t4 0x234:rd:t7:t6 0x254:rt:t4:t5 0x258:rt:t4:t5 0x25c:rs:t7:t6 0x260:rt:t4:t5 0x264:rt:t4:t5 0x27c:rt:t6:t7 0x280:rt:t6:t7 0x288:rt:t6:t7 0x294:rt:t6:t7 0x2b0:rd:t4:t5 0x2b8:rs:t4:t5 */
#include "types.h"

typedef struct { s16 vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern u8 *taco_actor_rec;

extern void srand(s32);
extern s32 rand(void);
extern s32 taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_spawn_rand(void)
{
    SVECTOR dir;
    SVECTOR pos;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 h;

    srand(taco_phase_cnt + 123456);
    r1 = rand();
    r2 = rand();
    pos.vx = r2 % 5000 + *(s16 *)(taco_actor_rec + 116) + r1 % 3000 - 4000;
    r3 = rand();
    r4 = rand();
    pos.vy = r4 % 2500 + *(s16 *)(taco_actor_rec + 118) + r3 % 1500 - 2000;
    pos.vz = -32768;
    dir.vx = rand() % 4096;
    dir.vy = rand() % 4096;
    dir.vz = rand() % 4096;
    dir.pad = rand() % 7;
    dir.vx = rand() % 3;
    if (rand() % 3 == 0) {
        h = taco_actor_alloc(0, 1, dir, pos, dir, dir.vx * 4 + 4, 16);
    } else {
        h = taco_actor_alloc(0, 1, dir, pos, dir, dir.vx * 4 + 4, 0);
    }
    *(taco_actor_rec + h * 136 + 132) = dir.vx + 3;
}
