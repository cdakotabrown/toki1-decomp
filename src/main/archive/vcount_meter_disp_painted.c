/* Function: vcount_meter_disp */
/* Address: 0x80044648 */
/* Size: 708 bytes (177 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80044648.py */
#include "types.h"

extern s32 vblank_function_tbl0;
extern s32 vblank_function_tbl1;
extern s32 vblank_function_tbl2;
extern s32 vblank_function_tbl3;

typedef struct {
    s16 a;
    s16 b;
} Pair;

typedef struct {
    Pair p1;
    Pair p2;
} Rect4;

extern void box_set(s32 a0, Rect4 r, s32 a3);

void vcount_meter_disp(void) {
    s32 colors[4];
    Rect4 rect;
    s32 *cptr;
    s32 val;
    s32 idx;
    s32 *src;
    s32 ypos;
    s32 quotient;
    s32 divisor;

    divisor = 264;

    colors[0] = vblank_function_tbl0 / divisor;
    colors[1] = vblank_function_tbl1 / divisor;
    colors[2] = vblank_function_tbl2 / divisor;
    colors[3] = vblank_function_tbl3 / divisor;

    cptr = colors;
    do {
        val = *cptr;
        if (val >= 3) {
            *cptr = 0x800000;
        } else if (val >= 2) {
            *cptr = 0x808000;
        } else if (val > 0) {
            *cptr = 0x8000;
        } else {
            *cptr = 128;
        }
        cptr++;
    } while ((u32)cptr < (u32)&rect);

    rect.p2.a = 4;
    cptr = colors;
    idx = 0;
    src = &vblank_function_tbl0;
    ypos = 140;

    do {
        rect.p1.a = ypos;
        quotient = (s16)((*src << 4) / divisor);
        rect.p1.b = 76 - quotient;
        rect.p2.b = quotient;
        box_set(idx + 6, rect, *cptr);
        idx++;
        cptr++;
        ypos += 4;
        src++;
    } while (idx != 4);

    rect.p1.a = 136;
    rect.p2.b = 16;
    rect.p1.b = 60;
    box_set(idx + 6, rect, 0x808080);
}
