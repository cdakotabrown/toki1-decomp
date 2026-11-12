/* Address: 0x80136F7C */
/* Size: 64 bytes (16 insns) */
#include "types.h"

extern void taiiku_racer_seat_step();
extern void taiiku_slot_tbl_set();
extern void taiiku_racer_speed_set();
extern void taiiku_racers_approach();
extern void taiiku_racer_frame_set();

void taiiku_racers_step(void)
{
    taiiku_racer_seat_step();
    taiiku_slot_tbl_set();
    taiiku_racer_speed_set();
    taiiku_racers_approach();
    taiiku_racer_frame_set();
}
