/* @SymbolSize: 132 */
/* name_pad_advance - NAME_ENT overlay, Address: 0x80139D30, Size: 132 bytes (33 insns) */
#include "types.h"

extern s32 pad_trig;
extern s32 name_ent_view_page;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void name_pad_advance(void)
{
    s32 pad;

    pad = pad_trig;
    if (pad & 0x10) {
        name_ent_view_page = name_ent_view_page + 1;
        return;
    }
    if (pad & 0x20) {
        if (cursor_cw_x < 32 && cursor_cw_x >= -63 &&
            cursor_cw_y < 28 && cursor_cw_y >= 13) {
            name_ent_view_page = name_ent_view_page + 1;
        }
    }
}
