/* Function: icon_menu_set */
/* Address: 0x80065670 */
/* Size: 1072 bytes (268 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80065670.py */
#include "types.h"

extern s8 icon_num;
extern s16 icon_xy[];
extern u8 class_w[];

extern s16 cw94_anim;
extern s16 cw94_x;
extern s16 cw94_y;
extern s16 D_80128680;
extern s16 cw78_x;
extern s16 cw78_y;
extern s8 D_8012866B;
extern s8 D_80128AAB;

extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *arg4, s16 *arg5);

void icon_menu_set(s32 a0) {
    s32 t0;
    s16 spC0[16];
    s16 spA0[16];
    s16 sp80[16];
    s16 sp60[16];

    if (a0 != 15) {
        a0 = 14;
        icon_num = 14;
    }

    if (a0 <= 0) {
        return;
    }

    for (t0 = 0; t0 < a0; t0++) {
        spC0[t0] = icon_xy[t0 * 2] - 10;
        spA0[t0] = icon_xy[t0 * 2 + 1] - 10;
        sp80[t0] = 20;
        sp60[t0] = 16;
    }

    t0 = 0;
    menu_set(0, a0 & 0xff, spC0, spA0, sp80, sp60);

    if (a0 <= 0) {
        goto phase3;
    }

    for (t0 = 0; t0 < a0; t0++) {
        *(s16 *)(class_w + t0 * 68 + 5462) = (s16)t0;
        *(s16 *)(class_w + t0 * 68 + 5478) = icon_xy[t0 * 2];
        *(s16 *)(class_w + t0 * 68 + 5482) = icon_xy[t0 * 2 + 1];
    }

phase3:
    t0 = 0;

    if (a0 <= 0) {
        goto end;
    }

    for (t0 = 0; t0 < a0; t0++) {
        *(s16 *)(class_w + t0 * 68 + 4376) = 2;
        *(s16 *)(class_w + t0 * 68 + 4390) = icon_xy[t0 * 2];
        *(s16 *)(class_w + t0 * 68 + 4394) = icon_xy[t0 * 2 + 1];
    }

end:
    if (a0 != 15) {
        D_8012866B = 0;
        D_80128AAB = 0;
        return;
    }

    {
        s16 v0val = icon_xy[4];
        s16 v1val = icon_xy[5];
        cw94_anim = 15;
        cw94_x = v0val;
        cw94_y = v1val;
        D_80128680 = 2;
        cw78_x = v0val;
        cw78_y = v1val;
    }
}
