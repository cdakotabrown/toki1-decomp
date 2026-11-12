/* Address: 0x8014ACE0 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void draw2d3d(s32, s32);
extern void date_step_next(void);

void date_disp2(void)
{
    draw2d3d(1, 0);
    date_step_next();
}
