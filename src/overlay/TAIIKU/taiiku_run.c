/* @SymbolSize: 56 */
/* Address: 0x80136AEC */
/* Size: 56 bytes (14 insns) */
#include "types.h"

extern void taiiku_accel_set();
extern void taiiku_speed_set();
extern void taiiku_pos_set();
extern void taiiku_run_frame_set();

void taiiku_run(void)
{
    taiiku_accel_set();
    taiiku_speed_set();
    taiiku_pos_set();
    taiiku_run_frame_set();
}
