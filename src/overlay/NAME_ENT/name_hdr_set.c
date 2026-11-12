/* Size: 312 bytes */
/* @SymbolSize: 312 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern u8 cw97_disp;
extern u8 cal_cw_disp;
extern CLASS_W class_w[];

s32 name_hdr_set(void)
{
    s32 sel0;
    s32 sel1;
    s32 sel2;
    s32 sel3;
    u32 col;

    sel0 = 0;
    sel1 = 0;
    if (now_sub_step == 0) {
        return 0;
    }
    sel2 = 0;
    sel3 = 0;
    col = now_sub_sub_step;
    if (col < 160U) {
        sel0 = 1;
    } else if (col < 192U) {
        sel1 = 1;
    } else if (col < 224U) {
        sel2 = 1;
    } else {
        sel3 = 1;
    }

    if (sel0) {
        class_w[99].disp_flag |= 0x80;
    } else {
        class_w[99].disp_flag &= ~0x80;
    }
    if (sel1) {
        cw97_disp |= 0x80;
    } else {
        cw97_disp &= ~0x80;
    }
    if (sel2) {
        cal_cw_disp |= 0x80;
    } else {
        cal_cw_disp &= ~0x80;
    }
    if (sel3) {
        class_w[98].disp_flag |= 0x80;
    } else {
        class_w[98].disp_flag &= ~0x80;
    }
}
