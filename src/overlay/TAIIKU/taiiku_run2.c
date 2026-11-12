/* Address: 0x8013CF44 */
/* Size: 56 bytes (14 insns) */
#include "types.h"

extern void taiiku_steer_set();
extern void taiiku_balance_advance();
extern void runner_move();
extern s32 taiiku_camera_mtx_set();

void taiiku_run2(void)
{
    taiiku_steer_set();
    taiiku_balance_advance();
    runner_move();
    taiiku_camera_mtx_set();
}
