/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --from 48 --to 68 --from 52 --to 64 --from 36 --to 48 */
/* @PostProc: patch_insn_reg_field 0x1ec:rt:t4:t6 0x1fc:rt:t6:t2 0x208:rt:t1:t5 0x214:rt:t3:t8 0x220:rt:v0:t4 0x224:rt:t4:t6 0x228:rt:v0:t4 0x22c:rt:v0:t4 0x23c:rt:v0:t7 0x240:rt:t6:t2 0x244:rd:t7:t8 0x244:rt:v0:t7 0x248:rd:t8:t1 0x248:rt:t7:t8 0x24c:rd:t8:t1 0x24c:rs:t8:t1 0x24c:rt:t7:t8 0x250:rd:t8:t1 0x250:rt:t8:t1 0x258:rt:t8:t1 0x25c:rt:v0:t9 0x260:rt:t1:t5 0x264:rd:t2:t3 0x264:rt:v0:t9 0x268:rd:t9:t4 0x268:rt:t2:t3 0x26c:rd:t9:t4 0x26c:rs:t9:t4 0x26c:rt:t2:t3 0x270:rd:t9:t4 0x270:rt:t9:t4 0x278:rt:t9:t4 0x27c:rt:v0:t6 0x280:rt:t3:t8 0x284:rd:t4:t7 0x284:rt:v0:t6 0x288:rd:t4:t7 0x288:rs:t4:t7 0x288:rt:v0:t6 0x28c:rd:t4:t7 0x28c:rt:t4:t7 0x290:rt:t4:t7 0x2a0:rt:t5:t1 0x2a4:rt:t8:t3 0x2a8:rt:t5:t1 0x2ac:rs:t8:t3 0x2ac:rt:t8:t3 0x2b8:rd:t1:t4 0x2b8:rs:t8:t3 0x2bc:rd:t6:t2 0x2c0:rd:t7:t9 0x2c0:rt:t6:t2 0x2c4:rs:t1:t4 0x2c4:rt:t7:t9 0x2d4:rt:t2:t5 0x2d8:rt:t6:t2 0x2dc:rd:t9:t6 0x2dc:rt:t2:t5 0x2e0:rt:t9:t6 0x2e4:rs:t6:t2 0x2e4:rt:t6:t2 0x2ec:rt:t7:t9 0x2f0:rd:t8:t3 0x2f0:rs:t6:t2 0x2f8:rd:t3:t7 0x2fc:rd:t4:t8 0x2fc:rt:t3:t7 0x300:rs:t4:t8 0x300:rt:t5:t1 0x304:rs:t8:t3 0x304:rt:t5:t1 0x308:rs:t7:t9 0x308:rt:t7:t9 0x310:rt:t7:t9 0x320:rd:t1:t4 0x324:rs:t1:t4 0x32c:rt:t2:t5 0x330:rt:t4:t8 0x334:rt:t2:t5 0x338:rt:t9:t6 0x340:rt:t9:t6 0x344:rt:t3:t7 0x34c:rt:t3:t7 0x350:rs:t4:t8 0x354:rs:t4:t8 0x364:rs:t4:t8 0x364:rt:t5:t1 0x368:rs:t4:t8 0x368:rt:t5:t1 0x370:rt:t5:t1 0x374:rt:t5:t1 0x384:rt:t8:t3 0x388:rs:t8:t3 0x388:rt:t8:t3 0x394:rt:t8:t3 0x3ac:rt:t7:t9 0x3b4:rs:t7:t9 0x3b4:rt:t1:t4 0x3bc:rt:t1:t4 0x3c8:rt:t9:t6 0x3d0:rs:t9:t6 0x3d0:rt:t9:t6 0x3d4:rt:t2:t5 0x3d8:rd:t3:t7 0x3d8:rs:t9:t6 0x3dc:rs:t3:t7 0x3dc:rt:t2:t5 */
/* @SymbolStart: 0x801585E0 */
/* @SymbolSize: 992 */
/* Address: 0x801585F0 (real entry 0x801585E0) */
#include "types.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SV;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void taco_shot_spawn(SV, s32, s32);
extern void taco_type12_retire(void);

extern u8 *taco_actor_rec;

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))
#define ACB(n) (*(u8 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_step11(s32 a0)
{
    SV sv;
    s32 k;
    s32 ang;
    s32 t;

    switch (AC(124)) {
    case 0:
        if ((AC(106) & 1) != 0) {
            AC(116) = -6000;
        } else {
            AC(116) = 6000;
        }
        if ((AC(106) & 0x10) != 0) {
            AC(118) = -2200;
        } else {
            AC(118) = 800;
        }
        AC(120) = 0;
        AC(124) = AC(124) + 1;
        break;
    case 1:
        if ((AC(106) & 1) != 0) {
            AC(116) = AC(122) * 30 - 6000;
        } else {
            AC(116) = 6000 - AC(122) * 30;
        }
        if ((AC(106) & 0x10) != 0) {
            AC(118) = AC(122) * 5 - 2200;
        } else {
            AC(118) = 800 - AC(122) * 5;
        }
        AC(120) = -AC(122) * 30;
        if (AC(122) >= 201) {
            AC(122) = 0;
            AC(124) = AC(124) + 1;
        }
        break;
    case 2:
        switch (AC(106)) {
        case 0:
            t = AC(122);
            k = -500;
            ang = t * 20;
            break;
        case 1:
            t = AC(122);
            k = -500;
            ang = -t * 20;
            break;
        case 16:
            t = AC(122);
            k = 500;
            ang = -t * 20;
            break;
        case 17:
            t = AC(122);
            k = 500;
            ang = t * 20;
            break;
        }
        AC(116) = (rsin(ang) * k) >> 12;
        AC(118) = ((rcos(ang) * -k) >> 12) - 700;
        if (AC(122) % 20 == 0) {
            sv.vx = AC(116);
            sv.vy = AC(118);
            sv.vz = AC(120);
            taco_shot_spawn(sv, 256, 192);
        }
        if (AC(122) >= 410) {
            AC(124) = AC(124) + 1;
        }
        break;
    case 3:
        taco_type12_retire();
        ACB(2) = 254;
        break;
    }
}
