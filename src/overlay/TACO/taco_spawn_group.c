/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x801563CC */
/* Size: 1344 bytes (336 instructions) */
/* @PostProc: patch_insn_reg_field 0x68:rs:t7:t8 0x5c:rt:t7:t8 0x60:rs:t7:t8 0x60:rt:t7:t8 0x68:rt:t8:t9 0x6c:rd:t9:t7 0x70:rs:t8:t7 0xa8:rt:t5:t6 0xac:rs:t5:t6 0xac:rt:t5:t6 0xb4:rs:t5:t6 0xb4:rt:t6:t8 0xb8:rd:t7:t5 0xbc:rd:t8:t7 0xbc:rs:t6:t5 0xbc:rt:t7:t8 0xcc:rs:t8:t7 0x100:rt:t7:t8 0x104:rt:t7:t8 0x110:rd:t5:t6 0x114:rs:t5:t6 0x114:rt:t6:t5 0x11c:rt:t6:t5 0x120:rt:t8:t7 0x124:rs:t8:t7 0x124:rt:t8:t7 0x13c:rs:t8:t7 0x168:rt:t6:t5 0x188:rt:t5:t6 0x18c:rt:t5:t6 0x194:rt:t5:t6 0x198:rt:t5:t6 0x19c:rs:t6:t5 0x1a0:rs:t6:t5 0x1a4:rt:t8:t7 0x1b0:rs:t6:t5 0x1b4:rs:t6:t5 0x1c4:rt:t8:t7 0x200:rt:t6:t5 0x220:rt:t7:t8 0x224:rt:t7:t8 0x22c:rt:t7:t8 0x230:rt:t7:t8 0x234:rs:t6:t5 0x238:rs:t6:t5 0x248:rs:t6:t5 0x248:rt:t8:t7 0x24c:rs:t6:t5 0x24c:rt:t8:t7 0x254:rt:t8:t7 0x268:rt:t8:t7 0x29c:rd:t5:t6 0x2a0:rs:t5:t6 0x2b0:rt:t7:t8 0x2b4:rs:t7:t8 0x2bc:rs:t7:t8 0x2bc:rt:t7:t8 0x2c0:rt:t7:t8 0x2dc:rt:t8:t7 0x2e0:rt:t8:t7 0x2e8:rt:t8:t7 0x2ec:rt:t8:t7 0x320:rt:t7:t8 0x33c:rt:t7:t8 0x354:rd:t8:t7 0x35c:rd:t8:t7 0x35c:rs:t8:t7 0x360:rd:t8:t7 0x360:rt:t8:t7 0x364:rt:t6:t5 0x368:rt:t8:t7 0x370:rt:t6:t5 0x37c:rt:t5:t6 0x380:rt:t5:t6 0x3b8:rt:t8:t7 0x3d8:rs:t8:t7 0x3dc:rs:t8:t7 0x3ec:rs:t8:t7 0x3f0:rs:t8:t7 0x414:rt:t5:t6 0x418:rt:t5:t6 0x420:rt:t5:t6 0x434:rt:t5:t6 0x44c:rt:t7:t8 0x450:rd:t6:t5 0x458:rs:t6:t5 0x458:rt:t7:t8 0x46c:rd:t8:t7 0x470:rs:t8:t7 0x494:rt:t5:t6 0x498:rs:t5:t6 0x49c:rs:t5:t6 0x4ac:rs:t5:t6 0x4b0:rs:t5:t6 0x4b4:rt:t8:t7 0x4d4:rt:t6:t5 0x4d8:rt:t6:t5 0x4e0:rt:t6:t5 0x4e4:rt:t6:t5 0x4e8:rs:t8:t7 0x4ec:rs:t8:t7 0x4fc:rs:t8:t7 0x500:rs:t8:t7 0x520:rt:t5:t6 0x528:rs:t5:t6 0x528:rt:t5:t6 0x538:rd:t7:t8 0x538:rs:t5:t6 0x53c:rs:t7:t8 */
#include "types.h"

typedef struct { s16 vx, vy, vz, pad; } SVECTOR;

extern s32 taco_phase_cnt;
extern volatile s32 taco_actor_rec;

extern void srand(s32);
extern s32 rand(void);
extern s32 taco_actor_alloc(s32, s32, SVECTOR, SVECTOR, SVECTOR, s32, s32);

void taco_spawn_group(s32 arg0)
{
    SVECTOR sp88;
    SVECTOR sp80;
    SVECTOR sp72;
    s32 idx1;
    s32 tmp;
    s32 idx2;
    s32 idx3;

    srand(taco_phase_cnt);

    if (arg0 == 0) {
        tmp = rand();
        sp80.vx = rand() % 4000
                + *(s16 *)((s32)taco_actor_rec + 116)
                + tmp % 2000 - 3000;
        tmp = rand();
        sp80.vy = rand() % 2500
                + *(s16 *)((s32)taco_actor_rec + 118)
                + tmp % 1500 - 2000;
        sp80.vz = rand() % 20000 + 32768;
        sp88.vx = rand() % 30 + 40;
        sp72.vy = 2048;
        sp72.vx = 0;
        sp72.vz = 0;
        sp88.vy = rand() % 2;
        if (taco_phase_cnt < 240) {
            taco_actor_alloc(7, 7, sp88, sp80, sp72, 5, 80);
            return;
        }
        taco_actor_alloc(7, 7, sp88, sp80, sp72, 5, 0);
        return;
    }

    sp80.vy = -900;
    sp80.vz = -26000;
    sp72.vy = 2048;
    sp72.vx = 0;
    sp72.vz = 0;
    sp88.vx = rand() % 20 + 30;
    sp80.vx = 0;
    sp88.vy = rand() % 2;
    idx1 = taco_actor_alloc(7, 7, sp88, sp80, sp72, 20, 0);
    *(s8 *)((s32)taco_actor_rec + idx1 * 136 + 132) = 4;

    sp88.vx = rand() % 20 + 30;
    sp80.vx = -1000;
    sp88.vy = rand() % 2;
    idx2 = taco_actor_alloc(7, 7, sp88, sp80, sp72, 20, 0);
    *(s8 *)((s32)taco_actor_rec + idx2 * 136 + 132) = 4;

    sp88.vx = rand() % 20 + 30;
    sp80.vx = 1000;
    sp88.vy = rand() % 2;
    idx3 = taco_actor_alloc(7, 7, sp88, sp80, sp72, 20, 0);
    *(s8 *)((s32)taco_actor_rec + idx3 * 136 + 132) = 4;
}
