/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 28 --to 48 --from 40 --to 56 --from 42 --to 58 --from 44 --to 60 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: patch_insn_reg_field 0x150:rt:t7:t8 0x154:rt:t9:t7 0x158:rd:t8:t9 0x158:rt:t7:t8 0x15c:rd:t8:t9 0x15c:rs:t8:t9 0x15c:rt:t7:t8 0x160:rd:t8:t9 0x160:rt:t8:t9 0x164:rd:t8:t9 0x164:rs:t8:t9 0x164:rt:t7:t8 0x168:rd:t8:t9 0x168:rt:t8:t9 0x16c:rs:t8:t7 0x1ac:rt:t8:t7 0x1b0:rt:t7:t8 0x1b4:rt:t7:t8 0x1b8:rs:t8:t7 0x1b8:rt:t8:t7 0x1c0:rt:t8:t7 */
/* @SymbolStart: 0x80155F80 */
/* @SymbolSize: 480 */
/* Address: 0x80155F90 (real entry 0x80155F80) */
#include "types.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SV;

extern void taco_shot_spawn(SV, s32, s32);
extern void taco_actor_exit(void);

extern u8 *taco_actor_rec;

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_step5(s32 a0)
{
    SV sv;
    s32 c;

    c = AC(126);

    switch (c) {
    case 0:
        c = AC(122);
        if (c % 7 == 0) {
            sv.vx = AC(116);
            sv.vy = AC(118);
            sv.vz = AC(120);
            taco_shot_spawn(sv, 256, 192);
            c = AC(122);
        }
        if (c == 60) {
            if (*(s16 *)(taco_actor_rec + 116) < AC(124)) {
                AC(124) = 1;
            } else {
                AC(124) = -1;
            }
            AC(122) = 0;
            AC(126) = AC(126) + 1;
        }
        break;
    case 1:
        AC(102) = AC(102) + AC(124) * 50;
        if (AC(122) >= 41) {
            AC(126) = AC(126) + 1;
        }
        break;
    case 2:
        AC(120) = AC(120) - 800;
        if (AC(120) < -30000) {
            taco_actor_exit();
        }
        break;
    }
}
