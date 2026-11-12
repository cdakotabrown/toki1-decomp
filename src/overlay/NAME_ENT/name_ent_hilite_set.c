/* @SymbolSize: 332 */
#include "types.h"

extern u8 class_w[][68];
extern s16 cw119_y;
extern s32 name_ent_tab_no;
extern s32 name_ent_tab_hit;

void name_ent_hilite_set(void)
{
    s32 i;
    s32 band;

    for (i = 0; i < 5; i++) {
        if (i == name_ent_tab_no) {
            class_w[i][7755] |= 0x80;
        } else {
            class_w[i][7755] &= 0x7F;
        }
    }
    band = name_ent_tab_hit;
    if (band >= 0) {
        cw119_y = band * 32 - 40;
    } else {
        cw119_y = -180;
    }
}
