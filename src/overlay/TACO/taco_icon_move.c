/* Address: 0x80143524 */
/* Size: 144 bytes (36 insns) */
#include "types.h"

extern s32 pad_trig;
extern u8 menu_num;
extern u8 menu_mode;
extern s8 now_icon;

void taco_icon_move(void)
{
    s32 in;
    s32 lim;

    in = pad_trig;
    menu_mode = 1;

    if ((in & 0x1000) != 0) {
        if (now_icon != 0) {
            now_icon = now_icon - 1;
        } else {
            now_icon = menu_num - 1;
        }
    }

    if ((in & 0x4000) != 0) {
        lim = menu_num - 1;
        if ((u32)now_icon < (u32)lim) {
            now_icon = now_icon + 1;
        } else {
            now_icon = 0;
        }
    }
}
