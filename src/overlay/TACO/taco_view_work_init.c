/* Address: 0x80143D60 */
/* Size: 28 bytes (7 insns) */
#include "types.h"

s16 taco_grid_ofs[4] = {0, 0, 0, 0};

void taco_view_work_init(void)
{
    taco_grid_ofs[0] = 0;
    taco_grid_ofs[1] = 1024;
    taco_grid_ofs[2] = 0;
    taco_grid_ofs[3] = 0;
}
