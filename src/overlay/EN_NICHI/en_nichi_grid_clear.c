/* Size: 140 bytes */
/* @SymbolSize: 140 */
#include "types.h"

typedef struct {
    s16 unk0;
    s16 x;
    s16 unk4;
    s16 y;
} EN_NICHI_CELL;

extern EN_NICHI_CELL en_nichi_grid_buf[15][12];
extern s32 en_nichi_sway_x;
extern s32 en_nichi_sway_y;

void en_nichi_grid_clear(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 15; j++) {
            en_nichi_grid_buf[j][i].x = 0;
            en_nichi_grid_buf[j][i].y = 0;
        }
    }
    en_nichi_sway_x = 0;
    en_nichi_sway_y = 0;
}
