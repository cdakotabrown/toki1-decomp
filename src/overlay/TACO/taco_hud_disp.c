/* Address: 0x8013F340 */
/* Size: 56 bytes (14 insns) */
#include "types.h"

extern void taco_board_disp();
extern void taco_status_disp();
extern void taco_sign_disp();
extern void taco_gauge_disp();

void taco_hud_disp(void)
{
    taco_board_disp();
    taco_status_disp();
    taco_sign_disp();
    taco_gauge_disp();
}
