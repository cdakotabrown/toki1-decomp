/* @PostProc: scheduler_normalizer */
/* @SymbolSize: 116 */
/* ending_grid_scroll - ENDING overlay, Address: 0x80136C7C, Size: 116 bytes (29 insns) */
#include "types.h"

extern u8 Sprites[];
extern s16 bustup2_cw_y;
extern s16 bustup_cw_y;

void ending_grid_scroll(s16 delta)
{
    s16 i;
    u8 *rec;

    for (i = 0; i < 12; i++) {
        rec = Sprites + i * 36;
        *(s16 *)(rec + 150) += delta;
    }
    bustup2_cw_y += delta;
    bustup_cw_y += delta;
}
