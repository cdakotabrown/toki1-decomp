/* @PostProc: patch_stack_complete --old 56 --new 72 */
/* @PostProc: remap_stack_slots --from 36 --to 56 --from 48 --to 64 --from 50 --to 66 --from 52 --to 68 */
/* @PostProc: patch_insn_reg_field 0x30:rt:v1:a1 0x38:rt:v1:a1 0x4c:rs:t8:a1 0x4c:rt:v1:t8 0x60:rt:a1:v1 0x68:rs:a1:v1 0x84:rt:a1:v1 0x90:rs:a1:v1 */
/* @SymbolStart: 0x80159F70 */
/* @SymbolSize: 516 */
/* Address: 0x80159F80 (real entry 0x80159F70) */
#include "types.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SV;

extern s32 abs(s32);
extern void taco_shot_spawn(SV, s32, s32);

extern u8 *taco_actor_rec;

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_shock(s32 a0)
{
    SV sv;
    s32 c;

    c = AC(116);
    AC(116) = c + (-c) / 50;

    if (AC(122) >= 181) {
        AC(122) = 0;
    }
    if (AC(122) % 10 != 0) {
        return;
    }
    if (abs(AC(116)) >= 1000) {
        return;
    }

    sv.vx = AC(116);
    sv.vy = AC(118);
    sv.vz = AC(120);

    switch (AC(118)) {
    case -1200:
        if (AC(122) < 60) {
            taco_shot_spawn(sv, 384, 192);
        }
        break;
    case -700:
        c = AC(122);
        if (c >= 61 && c < 120) {
            taco_shot_spawn(sv, 384, 130);
        }
        break;
    case -200:
        if (AC(122) >= 121) {
            taco_shot_spawn(sv, 384, 192);
        }
        break;
    }
}
