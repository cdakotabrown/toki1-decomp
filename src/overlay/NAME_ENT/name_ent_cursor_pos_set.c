/* name_ent_cursor_pos_set - NAME_ENT overlay, Address: 0x80134EC4, Size: 180 bytes (45 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 name_ent_lower_col;
extern s32 name_ent_lower_row;
extern s32 name_ent_upper_col;
extern s32 name_ent_upper_row;
extern CLASS_W class_w[];


void name_ent_cursor_pos_set(void)
{
    s32 x;
    s32 y;

    x = name_ent_upper_col;
    if (x >= 0) {
        y = name_ent_upper_row;
        if (y >= 0) {
            class_w[120].x.half[1] = (x * 16) - 113;
            class_w[120].y.half[1] = (y * 16) - 41;
            return;
        }
    }

    x = name_ent_lower_col;
    if (x >= 0) {
        y = name_ent_lower_row;
        if (y >= 0) {
            if ((x % 6) != 5) {
                class_w[120].x.half[1] = (x * 16) - 113;
                class_w[120].y.half[1] = (y * 16) + 63;
                return;
            }
        }
    }

    class_w[120].x.half[1] = 256;
    class_w[120].y.half[1] = 256;
}
