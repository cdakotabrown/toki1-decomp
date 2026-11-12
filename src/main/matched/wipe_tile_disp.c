/* Function: wipe_tile_disp */
/* Address: 0x8007D414 */
/* Size: 944 bytes (236 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8007D414.py */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 bg_change_tbl1;

extern void sprite_disp(s16, s16, s16, s16, s32, s32, s16, u8, u8, u8);
extern void goto_tpage(s32, s32, s32, s32, s32);

void wipe_tile_disp(s32 arg0) {
    s16 s0;
    s32 v0;
    s32 v1;
    s16 ten;
    s16 height;
    s16 s5;
    s32 s1;

    height = (s16)arg0;
    ten = 10;
    s5 = 0;
    s0 = 0;
    do {
        s1 = s5 << 3;
        do {
            v0 = s0 * ten;
            v1 = s1 + (s0 % 2) * 8;
            sprite_disp((s16)v0, (s16)v1, ten, height, v0, v1, 12, 128, 128, 128);
            s0 = (s16)(s0 + 2);
        } while (s0 < 16);
        s0 = 1;
        do {
            v0 = s0 * ten;
            v1 = s1 + (s0 % 2) * 8 - arg0;
            sprite_disp((s16)v0, (s16)v1, ten, height, v0, v1, 12, 128, 128, 128);
            s0 = (s16)(s0 + 2);
        } while (s0 < 16);
        s5 = (s16)(s5 + 1);
        s0 = 0;
    } while (s5 < 20);

    if (bg_change_tbl1 != 0) {
        goto_tpage(320, 256, 12, 0, 2);
    } else {
        goto_tpage(704, 0, 12, 0, 2);
    }

    s5 = 0;
    s0 = 16;
    do {
        s1 = s5 << 3;
        do {
            v0 = s0 * ten;
            v1 = s1 + (s0 % 2) * 8;
            sprite_disp((s16)v0, (s16)v1, ten, height, v0 - 128, v1, 12, 128, 128, 128);
            s0 = (s16)(s0 + 2);
        } while (s0 < 32);
        s0 = 17;
        do {
            v0 = s0 * ten;
            v1 = s1 + (s0 % 2) * 8 - arg0;
            sprite_disp((s16)v0, (s16)v1, ten, height, v0 - 128, v1, 12, 128, 128, 128);
            s0 = (s16)(s0 + 2);
        } while (s0 < 32);
        s5 = (s16)(s5 + 1);
        s0 = 16;
    } while (s5 < 20);

    if (bg_change_tbl1 != 0) {
        goto_tpage(480, 256, 12, 0, 2);
    } else {
        goto_tpage(864, 0, 12, 0, 2);
    }
}
