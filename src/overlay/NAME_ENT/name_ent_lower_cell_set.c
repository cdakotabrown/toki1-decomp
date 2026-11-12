/* @SymbolSize: 152 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
#include "types.h"

extern s16 cursor_cw_x;
extern volatile s16 cursor_cw_y;
extern s32 name_ent_lower_col;
extern s32 name_ent_lower_row;

s32 name_ent_lower_cell_set(void)
{
    s32 t;

    t = cursor_cw_x;
    if (t >= -119 && t < 152) {
        t = cursor_cw_y;
        if (t >= 57 && t < 104) {
            name_ent_lower_col = (cursor_cw_x + 120) / 16;
            name_ent_lower_row = (t - 56) / 16;
            return t;
        }
    }
    name_ent_lower_col = -1;
    name_ent_lower_row = -1;
    return t;
}
