/* @SymbolStart: 0x80137380 */
/* @SymbolSize: 736 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=23:t0,26:t4,29:t2 IDO52_UGEN_TEMP_REG=7:t8,12:t7,15:t9,20:t9,22:t5 IDO52_AS1_TIE_TAKE=422,2256,2264,2267 IDO52_AS1_TIE_REJECT=2249,2250 */
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
extern u8  _debug_number_show_str0;
extern u8  bunkaken_flags;
extern s32 taco_rec;
extern u8  cw159_disp;
extern s16 cw159_frame;
extern s16 cw159_scale_x;
extern s16 cw159_scale_y;
extern s16 cw159_x;
extern s16 cw159_y;

option_subgame_init()
{
    s32 i;
    Rec *p;
    s32 u0;
    s32 u2;
    s16 rectX[6];
    s16 rectY[6];
    s16 rectW[6];
    s16 rectH[6];
    s32 u1;
    s32 q0, q1;

    if (step_mode != 0) {
        cw159_scale_x = cw159_scale_x / 17 * 16;
        cw159_scale_y = cw159_scale_y / 17 * 16;
        if (cw159_scale_x < 4097) {
            if (cw159_scale_y) {
            }
            cw159_scale_x = 4096;
            cw159_scale_y = 4096;
            u0 = 8;
            u2 = 60;
            u1 = 0;
            for (i = 0; i < 2; i++) {
                cw96_active[i + 15].f24 = u2;
                rectX[u1] = -80;
                rectY[u1] = u0;
                cw96_active[i + 15].f42 = rectY[u1];
                cw96_active[i + 15].f38 = rectX[u1];
                u2 += 2;
                u0 += 16;
                u1++;
                rectW[u1 - 1] = 196;
                rectH[u1 - 1] = 16;
                cw96_active[i + 15].f3 |= 0x80;
            }
            cw96_active[15].f3 = 0;
            cw96_active[16].f3 = 0;
            if (_debug_number_show_str0 == 0) {
                if (!(bunkaken_flags & 1)) {
                    rectW[0] = 0;
                }
                if (!(taco_rec & 1)) {
                    rectW[1] = 0;
                }
            }
            menu_set(1, 2, rectX, rectY, rectW, rectH);
        }
    }
    if (step_mode == 0) {
        cw159_frame = 7;
        cw159_x = -160;
        cw159_y = -120;
        step_mode++;
    } else if (step_mode == 1) {
        for (i = 11; i < 15; i++) cw96_active[i].f3 &= ~0x80;
        cw96_active[10].f26 = cw96_active[10].f26 / 9 * 8;
        if (cw96_active[10].f26 < 9) {
            step_mode = 2;
        }
    } else {
        sub_sub_step_up();
    }
    cw96_active[10].f3 = 0;
    cw159_disp = 0;
    cw159_scale_x = 4096;
    cw159_scale_y = 4096;
    return 0;
}
