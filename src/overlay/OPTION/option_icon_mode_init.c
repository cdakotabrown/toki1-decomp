/* @SymbolStart: 0x80133D00 */
/* @SymbolSize: 664 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad0;
    u8  f1;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  pad6;
    u8  f7;
    u8  pad8[4];
    s32 f12;
    s32 f16;
    u8  pad20[4];
    s16 f24;
    s16 f26;
    s16 f28;
    s16 pad30[4];
    s16 f38;
    s16 pad40;
    s16 f42;
    u8  pad44[8];
    s32 f52;
    s32 f56;
    u8  pad60[7];
    u8  f67;
} Rec;

extern Rec cw96_active[];
extern u8  step_mode;
extern u8  menu_csr;
extern u8  icon_mode;
extern u8  icon_num;
typedef struct { s16 x; s16 y; } Pt;
extern Pt  icon_xy[];

option_icon_mode_init()
{
    s32 i;
    Rec *p;
    s32 u0;
    s16 rectX[4];
    s16 rectY[4];
    s16 rectW[4];
    s16 rectH[4];
    s32 u1;

    if (step_mode == 0) {
        cw96_active[10].f24 = 2;
        cw96_active[10].f38 = -160;
        cw96_active[10].f42 = -120;
        cw96_active[10].f3 = 0;
        cw96_active[10].f26 = 32767;
        cw96_active[10].f28 = 32767;
        step_mode = 1;
    } else {
        cw96_active[10].f26 = cw96_active[10].f26 / 17 * 16;
        cw96_active[10].f28 = cw96_active[10].f28 / 17 * 16;
        if (cw96_active[10].f26 < 4097) {
            cw96_active[10].f26 = 4096;
            cw96_active[10].f28 = 4096;
            for (i = 0; &cw96_active[i] < &cw96_active[3]; i++) {
                p = &cw96_active[i + 11];
                rectX[i] = -32;
                rectY[i] = i * 24 - 56;
                p->f38 = rectX[i]; p->f42 = rectY[i];
                rectW[i] = 80;
                rectH[i] = 16;
                p->f24 = i * 2 + 22;
                p->f3 |= 0x80;
            }
            menu_set(1, 3, rectX, rectY, rectW, rectH);
            switch (icon_mode % 3) {
            case 0:
            default:
                menu_csr = 0;
                break;
            case 1:
                menu_csr = 1;
                break;
            case 2:
                menu_csr = 2;
                break;
            }
            icon_init();
            for (i = 0; i < 16; i++) {
                icon_xy[i].x += 200;
            }
            icon_menu_set(icon_num);
            sub_sub_step_up();
        }
    }
    option_panels_scroll();
}
