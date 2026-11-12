/* @SymbolStart: 0x80153294 */
/* @SymbolSize: 540 */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_slot_step(u8 *, u8 *, s32, s32, s32);
extern void taco_reel(u8 *, u8 *, s32, s32, s32);
extern void taco_reel_step(u8 *, u8 *, s32, s32);
extern void taco_slots_anim(s32);

void taco_slots_main(void)
{
    s32 v;

    v = taco_actor_rec[2582];
    if (v == 1) {
        taco_slot_step(&taco_actor_rec[2451], &taco_actor_rec[2450], 1, 0, 4);
    } else if (v == 2) {
        taco_slot_step(&taco_actor_rec[2451], &taco_actor_rec[2450], 1, 0, 2);
    } else if (v == 3) {
        taco_reel_step(&taco_actor_rec[2451], &taco_actor_rec[2450], 1, 0);
    }

    v = taco_actor_rec[2718];
    if (v == 1) {
        taco_slot_step(&taco_actor_rec[2587], &taco_actor_rec[2586], 2, 7, 3);
    } else if (v == 2) {
        taco_reel(&taco_actor_rec[2587], &taco_actor_rec[2586], 2, 7, 4);
    } else if (v == 3) {
        taco_reel_step(&taco_actor_rec[2587], &taco_actor_rec[2586], 2, 7);
    }

    v = taco_actor_rec[2854];
    if (v == 1) {
        taco_slot_step(&taco_actor_rec[2723], &taco_actor_rec[2722], 3, 14, 2);
    } else if (v == 2) {
        taco_reel(&taco_actor_rec[2723], &taco_actor_rec[2722], 3, 14, 3);
    } else if (v == 3) {
        taco_reel_step(&taco_actor_rec[2723], &taco_actor_rec[2722], 3, 14);
    }

    if (taco_actor_rec[2446] == 1 && taco_actor_rec[2718] == 0 &&
        taco_actor_rec[2582] == 0 && taco_actor_rec[2854] == 0) {
        taco_slots_anim(4);
    }
}
