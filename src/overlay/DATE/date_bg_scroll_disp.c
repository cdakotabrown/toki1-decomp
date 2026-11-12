/* @SymbolStart: 0x80152268 */
/* @SymbolSize: 1052 */
/* @CompileFlags: -cckr */
#include "types.h"

extern volatile u32 frame_counter;
extern u8  date_bg_ptn[];

extern void date_bg_strip_disp(s16, s16, s16);

void date_bg_scroll_disp()
{
    short i;
    u32 w;

    if (frame_counter % 51 == 0) {
        for (i = 0; i < 12;) {
            date_bg_ptn[i++] = rand() % 4;
        }
    }

    w = 1000;
    i = (frame_counter << 4) % w;
    date_bg_strip_disp(i, 0, 9);
    i = (frame_counter << 4) % w;
    date_bg_strip_disp(i - 10, 1, 9);
    i = (frame_counter << 4) % w;
    date_bg_strip_disp(i - 20, 2, 9);

    i = ((frame_counter << 4) + 500) % w;
    date_bg_strip_disp(i, 6, 9);
    i = ((frame_counter << 4) + 500) % w;
    date_bg_strip_disp(i - 10, 7, 9);
    i = ((frame_counter << 4) + 500) % w;
    date_bg_strip_disp(i - 20, 8, 9);

    dtd_on_tpage(0, 0, 9, 3, 0);

    w = 800;
    i = w - (frame_counter << 4) % w;
    date_bg_strip_disp(i, 3, 13);
    i = w - (frame_counter << 4) % w;
    date_bg_strip_disp(i + 10, 4, 13);
    i = w - (frame_counter << 4) % w;
    date_bg_strip_disp(i + 20, 5, 13);

    i = w - ((frame_counter << 4) + 400) % w;
    date_bg_strip_disp(i, 9, 13);
    i = w - ((frame_counter << 4) + 400) % w;
    date_bg_strip_disp(i + 10, 10, 13);
    i = w - ((frame_counter << 4) + 400) % w;
    date_bg_strip_disp(i + 20, 11, 13);

    dtd_on_tpage(0, 0, 13, 3, 0);
}
