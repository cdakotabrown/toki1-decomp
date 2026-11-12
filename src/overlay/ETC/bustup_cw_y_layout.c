/* @SymbolStart: 0x8013E084 */
/* @SymbolSize: 328 */
/* Address: 0x8013E094 */
/* Size: 312 bytes (78 instructions) */
#include "types.h"
#include "tokimeki/sprite.h"

extern s32 etc_rec_girl;
extern s32 etc_rec_season;
extern s16 bustup2_cw_y;
extern s16 bustup_cw_y;
extern SPRITE_REC Sprites[];

void bustup_cw_y_layout(s16 **a0)
{
    s16 *v0;
    s16 x;
    s16 v1;
    s16 p0;
    s16 p1;

    if (etc_rec_girl == 10 && etc_rec_season == 1) {
        bustup2_cw_y = -21;
        bustup_cw_y = -21;
    } else {
        bustup2_cw_y = -13;
        bustup_cw_y = -13;
    }

    v0 = *a0;
    v1 = v0[0];
    if (v1 >= 6) {
        x = v0[2];
        p0 = v0[40];
        p1 = v0[33];
        Sprites[0].y = (bustup2_cw_y + p0) - x;
        Sprites[1].y = (bustup2_cw_y + p1) - x;
        v1 = v0[0];
    }
    if (v1 >= 4) {
        p0 = v0[19];
        p1 = v0[26];
        Sprites[2].y = (bustup2_cw_y + p0) - x;
        Sprites[3].y = (bustup2_cw_y + p1) - x;
        v1 = v0[0];
    }
    if (v1 >= 7) {
        p0 = v0[47];
        Sprites[4].y = (bustup2_cw_y + p0) - x;
        v1 = v0[0];
    }
    if (v1 == 8) {
        p1 = v0[54];
        Sprites[5].y = (bustup2_cw_y + p1) - x;
    }
}
