/* Function: icon_atari_check */
/* Address: 0x80065ACC */
/* Size: 424 bytes (106 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @CompileEnv: IDO52_FORCE_WEB_SPILL=123,124 IDO52_FORCE_WEB_COLOR=14:a3,21:t0,25:t1 IDO52_UGEN_TEMP_REG=10:t6 */
/* @CompileFlags: -Wc,-pic1 -cckr -Wo,-loopunroll,0 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
} PosEntry;

extern PosEntry icon_xy[];

s32 icon_atari_check(idx, offsetX, offsetY)
s32 idx;
s32 offsetX;
s32 offsetY;
{
    s32 x0, y0, x1, y1, x2, y2, x3, y3;
    s32 ox, oy;
    s32 i, ret;
    s32 pad[3];

    ret = 1;
    x0 = icon_xy[idx].x + offsetX;
    y0 = icon_xy[idx].y + offsetY;
    x1 = icon_xy[idx].x + offsetX + 19;
    y1 = icon_xy[idx].y + offsetY;
    x2 = icon_xy[idx].x + offsetX;
    y2 = icon_xy[idx].y + offsetY + 15;
    x3 = icon_xy[idx].x + offsetX + 19;
    y3 = icon_xy[idx].y + offsetY + 15;
    if (x0 < -160) {
        return 0;
    }
    if (x1 >= 161) {
        return 0;
    }
    if (y0 < -120) {
        return 0;
    }
    if (y1 >= 121) {
        return 0;
    }
    for (i = 0; i != 15; i++) {
        if (i != idx) {
            ox = icon_xy[i].x;
            oy = icon_xy[i].y;
            if (x0 >= ox && x0 < ox + 20 && y0 >= oy && y0 < oy + 16) {
                ret = 0;
            }
            if (x1 >= ox && x1 < ox + 20 && y1 >= oy && y1 < oy + 16) {
                ret = 0;
            }
            if (x2 >= ox && x2 < ox + 20 && y2 >= oy && y2 < oy + 16) {
                ret = 0;
            }
            if (x3 >= ox && x3 < ox + 20 && y3 >= oy && y3 < oy + 16) {
                ret = 0;
            }
        }
    }
    return ret;
}
