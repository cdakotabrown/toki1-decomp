/* Address: 0x8015777C */
/* Size: 884 bytes (221 instructions) */
/* @PostProc: patch_insn_reg_field 0x128:rt:t4:t1 0x12c:rt:t1:t7 0x130:rd:t5:t4 0x130:rt:t4:t1 0x134:rd:t8:t5 0x134:rt:t5:t4 0x138:rs:t8:t5 0x138:rt:t7:t8 0x140:rs:t1:t8 0x15c:rs:t6:t0 0x15c:rt:t0:t6 0x170:rt:t5:t1 0x174:rt:t4:t7 0x178:rd:t8:t4 0x178:rt:t5:t1 0x17c:rd:t1:t5 0x17c:rt:t8:t4 0x180:rs:t1:t5 0x180:rt:t7:t8 0x184:rt:t0:t6 0x188:rs:t4:t8 0x198:rd:t6:t0 0x1a0:rd:t6:t0 0x1b8:rt:t5:t4 0x1bc:rt:t9:t1 0x1c0:rd:t8:t5 0x1c0:rt:t5:t4 0x1c4:rd:t1:t8 0x1c4:rt:t8:t5 0x1c8:rs:t1:t8 0x1c8:rt:t4:t7 0x1cc:rs:t4:t7 0x1d0:rd:t7:t9 0x1d0:rt:t4:t7 0x1d4:rs:t4:t7 0x1d8:rd:t7:t9 0x1dc:rs:t7:t1 0x1e8:rt:t5:t4 0x200:rs:t5:t4 0x200:rt:t5:t4 0x208:rt:t5:t4 0x210:rt:t9:t5 0x214:rd:t1:t7 0x218:rd:t4:t1 0x218:rt:t1:t7 0x21c:rs:t4:t1 0x21c:rt:t7:t9 0x224:rs:t7:t5 0x250:rt:t5:t4 0x254:rt:t4:t1 0x258:rt:t5:t4 0x268:rt:t1:t7 0x270:rt:t1:t7 0x274:rs:t4:t1 0x278:rs:t4:t1 0x288:rs:t4:t1 0x28c:rs:t4:t1 0x2e4:rt:t5:t8 0x2ec:rd:t8:t7 0x2ec:rt:t5:t8 0x2f0:rd:t1:t5 0x2f0:rt:t8:t7 0x2f4:rs:t1:t5 0x2f4:rt:t7:t1 0x2fc:rs:t7:t4 0x2fc:rt:t4:t1 0x308:rt:t7:t4 0x310:rt:t0:t3 0x314:rt:t8:t0 0x318:rd:t3:t2 0x318:rt:t0:t3 0x31c:rd:t2:t8 0x31c:rt:t3:t2 0x320:rs:t2:t8 0x320:rt:t5:t7 0x328:rd:t1:t5 0x328:rs:t5:t0 0x328:rt:t8:t7 0x32c:rt:t1:t5 0x330:rs:t7:t4 0x330:rt:t7:t4 0x338:rt:t7:t4 0x33c:rt:t4:t1 0x344:rs:t4:t1 */
#include "types.h"

typedef struct { s16 vx, vy, vz, pad; } SV;
typedef struct { s32 a, b; } Pair;
typedef struct { Pair e[5]; } PairTab;
typedef struct { s32 e[5]; } LongTab;

extern PairTab taco_actor_step12_tbl0;
extern LongTab taco_actor_step12_tbl1;
extern u8 *taco_actor_rec;

extern void taco_shot_spawn(SV, s32, s32);
extern void taco_actor_exit(void);

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_step12(s32 a0)
{
    PairTab vel;
    LongTab spin;
    SV sv;

    vel = taco_actor_step12_tbl0;
    spin = taco_actor_step12_tbl1;

    switch (AC(124)) {
    case 0:
        AC(120) = AC(120) + 256;
        if (AC(120) < -7499) {
            return;
        }
        AC(122) = 0;
        AC(124) = AC(124) + 1;
        break;
    case 1:
        AC(116) = AC(116) + vel.e[AC(106)].a * AC(122) / 16;
        AC(118) = AC(118) + vel.e[AC(106)].b * AC(122) / 16;
        AC(120) = AC(120) + (-AC(122) * 256 + 4096) / 16;
        AC(100) = AC(100) + 64;
        AC(102) = AC(102) + spin.e[AC(106)];
        if (AC(122) % 20 == 0) {
            sv.vx = AC(116);
            sv.vy = AC(118);
            sv.vz = AC(120);
            taco_shot_spawn(sv, 128, 192);
        }
        if (AC(100) < 1024) {
            return;
        }
        AC(124) = AC(124) + 1;
        break;
    case 2:
        AC(116) = AC(116) + vel.e[AC(106)].a;
        AC(118) = AC(118) + vel.e[AC(106)].b;
        if (AC(118) < -4000 || AC(116) < -6000 || AC(116) > 6000) {
            taco_actor_exit();
        }
        break;
    }
}
