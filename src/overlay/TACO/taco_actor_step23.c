/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: remap_stack_slots --from 48 --to 56 --from 50 --to 58 --from 52 --to 60 --from 36 --to 48 */
/* @SymbolStart: 0x8015A690 */
/* @SymbolSize: 632 */
/* Address: 0x8015A6A0 (real entry 0x8015A690) */
#include "types.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SV;

extern void taco_shot_spawn(SV, s32, s32);
extern void taco_actor_exit(s32);

extern u8 *taco_actor_rec;

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))
#define ACB(n) (*(s8 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_step23(s32 a0)
{
    SV sv;
    s32 c;

    sv.vx = AC(116);
    sv.vy = AC(118);
    sv.vz = AC(120);

    switch (AC(124)) {
    case 0:
        AC(116) = 0;
        AC(118) = -700;
        AC(120) = -20000;
        AC(110) = 1;
        AC(124) = AC(124) + 1;
        break;
    case 1:
        AC(110) = AC(122) << 9;
        ACB(132) = AC(122) * 22 / 32;
        if (AC(110) >= AC(108)) {
            AC(124) = AC(124) + 1;
            AC(122) = 0;
        }
        break;
    case 2:
        if (AC(122) % 15 == 0) {
            taco_shot_spawn(sv, 256, 194);
        }
        if (AC(122) >= 601) {
            AC(124) = AC(124) + 1;
            AC(122) = 0;
        }
        break;
    case 3:
        AC(110) = (16 - AC(122)) << 9;
        ACB(132) = (16 - AC(122)) * 32 / 32;
        if (AC(110) <= 0) {
            taco_actor_exit(a0);
        }
        break;
    }
}
