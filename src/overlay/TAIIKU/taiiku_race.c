/* Address: 0x80147204 */
/* Size: 56 bytes (14 insns) */
#include "types.h"

extern void taiiku_slot_accel_set();
extern void taiiku_slot_speed_set();
extern void taiiku_slots_approach();
extern void taiiku_runner_anim();

void taiiku_race(void)
{
    taiiku_slot_accel_set();
    taiiku_slot_speed_set();
    taiiku_slots_approach();
    taiiku_runner_anim();
}
