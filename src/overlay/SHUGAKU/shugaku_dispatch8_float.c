/* @SymbolSize: 340 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    void (*fn[74])();
} Tbl;

extern Tbl shugaku_main8_tbl;
extern void date_fade_in();
extern void date_fade_out();
extern void shugaku_float_fade_out();
extern u8 now_sub_sub_step;
typedef struct {
    u8  pad0[3];
    u8  f3;
    u8  pad4[3];
    u8  f7;
    u8  pad8[60];
} Rec;

extern Rec class_w[];
extern u8 fade_bri;
extern u8 shugaku_float_on;

shugaku_dispatch8_float()
{
    s32 unk;
    Tbl tbl;

    tbl = shugaku_main8_tbl;
    tbl.fn[now_sub_sub_step](128);
    if (tbl.fn[now_sub_sub_step] == date_fade_in) {
        class_w[98].f3 |= 0x80;
    }
    if ((tbl.fn[now_sub_sub_step] == date_fade_out || tbl.fn[now_sub_sub_step] == shugaku_float_fade_out) && fade_bri < 8) {
        class_w[98].f3 &= ~0x80;
        class_w[99].f3 &= ~0x80;
    }
    if ((now_sub_sub_step >= 22 && now_sub_sub_step < 28) || (now_sub_sub_step >= 32 && now_sub_sub_step < 37)) {
        class_w[98].f7 = class_w[96].f7;
    }
    if (shugaku_float_on != 0) {
        shugaku_float();
    }
}
