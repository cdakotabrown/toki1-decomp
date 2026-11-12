/* Address: 0x80143E28 */
/* Size: 200 bytes (50 insns)  -- BYTE-PERFECT */
#include "types.h"

extern s16 taco_grid_ofs[];

extern s32 pad_get(void);

void taco_view_pad_move(void)
{
    s32 in;

    in = pad_get();

    if (in & 0x10000000) {
        taco_grid_ofs[1] = taco_grid_ofs[1] - 16;
    }
    if (in & 0x40000000) {
        taco_grid_ofs[1] = taco_grid_ofs[1] + 16;
    }
    if (in & 0x80000000) {
        taco_grid_ofs[0] = taco_grid_ofs[0] - 16;
    }
    if (in & 0x20000000) {
        taco_grid_ofs[0] = taco_grid_ofs[0] + 16;
    }
    if (in & 0x00040000) {
        taco_grid_ofs[2] = taco_grid_ofs[2] + 16;
    }
    if (in & 0x00010000) {
        taco_grid_ofs[2] = taco_grid_ofs[2] - 16;
    }
}
