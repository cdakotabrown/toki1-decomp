/* @RodataBase: 0x8015E1C4 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 --from 32 --to 48 */
/* @SymbolStart: 0x8015C2DC */
/* @SymbolSize: 924 */
/* Address: 0x8015C2EC (real entry 0x8015C2DC) */
#include "types.h"

extern void taco_actor_anim_frame(s32, u8 *);
extern void taco_actor_anim_frame2(s32);

extern u8 *taco_actor_rec;

extern u8 taco_anim11_scr[];
extern u8 taco_anim11b_scr[];
extern u8 taco_anim12_scr[];
extern u8 taco_anim12b_scr[];
extern u8 taco_anim13_scr[];
extern u8 taco_anim14_scr[];

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))
#define ACB(n) (*(u8 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_anim(s32 a0)
{
    if (ACB(3) == 0) {
        return;
    }

    switch (ACB(134)) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    case 11:
        AC(114) = AC(114) + 1;
        if (AC(114) < 6) {
            return;
        }
        AC(114) = 0;
        if (ACB(3) == 1) {
            taco_actor_anim_frame(a0, taco_anim11_scr);
            taco_actor_anim_frame(a0, taco_anim11b_scr);
        }
        AC(130) = AC(130) + 1;
        break;
    case 12:
        AC(114) = AC(114) + 1;
        if (AC(114) < 6) {
            return;
        }
        AC(114) = 0;
        if (ACB(3) == 1) {
            taco_actor_anim_frame(a0, taco_anim12_scr);
        } else {
            taco_actor_anim_frame(a0, taco_anim12b_scr);
        }
        AC(130) = AC(130) + 1;
        break;
    case 13:
        AC(114) = AC(114) + 1;
        if (AC(114) < 6) {
            return;
        }
        AC(114) = 0;
        if (ACB(3) == 2) {
            taco_actor_anim_frame(a0, taco_anim13_scr);
        }
        AC(130) = AC(130) + 1;
        break;
    case 14:
        AC(114) = AC(114) + 1;
        if (AC(114) < 6) {
            return;
        }
        AC(114) = 0;
        if (ACB(3) == 1) {
            taco_actor_anim_frame(a0, taco_anim14_scr);
        }
        AC(130) = AC(130) + 1;
        break;
    case 15:
        AC(114) = AC(114) + 1;
        if (AC(114) < 6) {
            return;
        }
        AC(114) = 0;
        if (ACB(3) == 1) {
            taco_actor_anim_frame2(a0);
        }
        AC(130) = AC(130) + 1;
        break;
    case 16:
        AC(114) = AC(114) + 1;
        if (AC(114) < 6) {
            return;
        }
        AC(114) = 0;
        if (ACB(3) == 1) {
            taco_actor_anim_frame2(a0);
        }
        AC(130) = AC(130) + 1;
        break;
    }
}
