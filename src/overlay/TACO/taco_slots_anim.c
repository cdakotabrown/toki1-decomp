/* @SymbolStart: 0x80152C30 */
/* @SymbolSize: 340 */
/* Address: 0x80152C30 (absorbed cross-prologue head) .. 0x80152D84 */
/* Size: 340 bytes (85 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_spin_row_disp(s32, s32, s32);
extern void taco_rows_disp(s32, s32);
extern void taco_reel_result_set(s32);

void taco_slots_anim(s32 arg0)
{
    if ((u32)arg0 < (u32)taco_actor_rec[2451]) {
        taco_spin_row_disp(1, 0, taco_actor_rec[2450]);
        taco_spin_row_disp(2, 7, taco_actor_rec[2450]);
        taco_spin_row_disp(3, 14, taco_actor_rec[2450]);
        taco_actor_rec[2450] = taco_actor_rec[2450] + 1;
        taco_actor_rec[2451] = 0;
    }

    if (taco_actor_rec[2450] == 80) {
        taco_actor_rec[2310] = 0;
    }

    if ((u32)taco_actor_rec[2450] >= 110U) {
        taco_actor_rec[2450] = 0;
        taco_actor_rec[2451] = taco_actor_rec[2450];
        taco_actor_rec[2446] = 0;
        taco_reel_result_set(1);
        taco_reel_result_set(2);
        taco_reel_result_set(3);
        taco_rows_disp(1, 0);
        taco_rows_disp(2, 7);
        taco_rows_disp(3, 14);
    } else {
        taco_actor_rec[2451] = taco_actor_rec[2451] + 1;
    }
}
