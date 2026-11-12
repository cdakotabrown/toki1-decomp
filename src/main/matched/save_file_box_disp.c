/* Function: save_file_box_disp */
/* Address: 0x8005CA74 */
/* Size: 364 bytes (91 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern u8 album_work[20];

extern void _sprite_set_box_shade(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 color, s32 style);

extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0);

void save_file_box_disp(void) {
    s32 i;
    u8 *pSlot;
    s32 color1;
    u8 val;

    pSlot = album_work;
    color1 = 0x808080;
    i = 0;

    do {
        val = pSlot[5];
        if (val == 0) {

            _sprite_set_box_shade(((i / 5) << 5) - 124, ((i % 5) << 4) - 64, 30, 15, 2, color1, 129);
        }
        val = pSlot[5];
        if (val == 255) {

            _sprite_set_box_shade(((i / 5) << 5) - 124, ((i % 5) << 4) - 64, 30, 15, 2, 0x8080, 129);
        }
        i++;
        pSlot++;
    } while (i != 14);

    dtd_on_tpage(0, 0, 2, 2, 0);
}
