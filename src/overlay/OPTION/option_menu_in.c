/* @CompileFlags: -cckr */
/* @PostProc: swap_instructions --offset1 0x11C --offset2 0x120 */
#include "types.h"
#include "tokimeki/class_w.h"

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

extern u8  step_mode;
extern u8  menu_csr;
extern u8  opt_flag;
extern u8  bunkaken_flags;
extern s32 taco_rec;
extern CLASS_W class_w[];

option_menu_in()
{
    s32 i;
    s32 u0;
    s32 u1;
    s32 u2;
    s16 rectX[6];
    s16 rectY[6];
    s16 rectW[6];
    s16 rectH[6];

    if (step_mode == 0) {
        ((Rec *)&class_w[96].active)[10].f24 = 6;
        ((Rec *)&class_w[96].active)[10].f38 = -160;
        ((Rec *)&class_w[96].active)[10].f42 = -120;
        ((Rec *)&class_w[96].active)[10].f3 = 0;
        ((Rec *)&class_w[96].active)[10].f26 = 32767;
        ((Rec *)&class_w[96].active)[10].f28 = 32767;
        step_mode = 1;
    } else {
        ((Rec *)&class_w[96].active)[10].f26 = ((Rec *)&class_w[96].active)[10].f26 / 17 * 16;
        ((Rec *)&class_w[96].active)[10].f28 = ((Rec *)&class_w[96].active)[10].f28 / 17 * 16;
        if (((Rec *)&class_w[96].active)[10].f26 < 4097) {
            ((Rec *)&class_w[96].active)[10].f26 = 4096;
            ((Rec *)&class_w[96].active)[10].f28 = 4096;
            for (i = 0; i < 4; i++) {
                rectX[i] = i / 7 * 128 - 64;
                rectW[i] = 96;
                rectH[i] = 16;
                ((Rec *)&class_w[96].active)[i + 11].f24 = i * 2 + 52;
                ((Rec *)&class_w[96].active)[i + 11].f38 = rectX[i];
                rectY[i] = i % 7 * 16 + 24;
                ((Rec *)&class_w[96].active)[i + 11].f42 = rectY[i];
                ((Rec *)&class_w[96].active)[i + 11].f3 |= 0x80;
            }
            if (!(bunkaken_flags & 1) && !(taco_rec & 1)) {
                rectW[0] = 0;
                ((Rec *)&class_w[96].active)[11].f3 = 0;
            }
            if (get_option_count() == 0) {
                ((Rec *)&class_w[96].active)[12].f3 = 0;
                rectW[1] = 0;
                rectW[2] = 0;
                ((Rec *)&class_w[96].active)[13].f3 = 0;
            }
            if (opt_flag == 0) {
                rectW[3] = 0;
                ((Rec *)&class_w[96].active)[14].f3 = 0;
            }
            menu_set(1, 4, rectX, rectY, rectW, rectH);
            menu_csr = 0;
            sub_sub_step_up();
        }
    }
    option_panels_scroll();
}
