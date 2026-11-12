/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80135E00 */
/* @SymbolSize: 948 */
/* @RodataBase: 0x8013CBDC */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  f02;
    u8  f03;
    u8  pad04[20];
    s16 f24;
    u8  pad26[12];
    s16 f38;
    u8  pad28[2];
    s16 f42;
    u8  pad2C[24];
} Rec68;

extern u8 csr_speed;
extern u8 menu_csr;
extern u8 step_mode;
extern Rec68 cw96_active[];
extern CLASS_W class_w[];

option_csr_speed_init()
{
    s32 i;
    s32 v;
    s32 w;
    s16 a112[6];
    s16 a100[6];
    s16 a88[6];
    s16 a76[6];

    if (step_mode == 0) {
        class_w[106].frame = 4;
        class_w[106].x.half[1] = -160;
        class_w[106].y.half[1] = -120;
        class_w[106].disp_flag = 0;
        class_w[106].scale_y = 32767;
        step_mode = 1;
        class_w[106].scale_x = 32767;
        return;
    }
    class_w[106].scale_x = class_w[106].scale_x / 17 * 16;
    class_w[106].scale_y = class_w[106].scale_y / 17 * 16;
    if (class_w[106].scale_x < 4097) {
        class_w[106].scale_y = 4096;
        class_w[106].scale_x = 4096;
        for (i = 0; i < 5; i++) {
            a112[i] = 16;
            a100[i] = (i - 1) * 24;
            a88[i] = 48;
            a76[i] = 16;
            cw96_active[11 + i].f24 = i * 2 + 12;
            cw96_active[11 + i].f38 = a112[i];
            cw96_active[11 + i].f42 = a100[i];
            cw96_active[11 + i].f03 |= 0x80;
        }
        menu_set(1, 5, a112, a100, a88, a76);
        switch (csr_speed % 10) {
        case 1:
        default:
            class_w[107].frame++;
            menu_csr = 0;
            break;
        case 2:
            class_w[108].frame++;
            menu_csr = 1;
            break;
        case 3:
            class_w[109].frame++;
            menu_csr = 1;
            break;
        case 4:
            class_w[110].frame++;
            menu_csr = 1;
            break;
        case 6:
            class_w[111].frame++;
            menu_csr = 1;
            break;
        }
        sub_sub_step_up();
    }
}
