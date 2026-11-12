/* Address: 0x8006625C */
/* Size: 520 bytes (130 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 f0;
    u8 f1;
    u8 f2;
    u8 f3;
    u8 f4;
    u8 f5;
    u8 pad6[6];
    s32 *f12;
    s32 *f16;
    s16 f20;
    s16 pad22;
    s16 f24;
    u8 pad26[12];
    s16 f38;
    s16 pad40;
    s16 f42;
    u8 pad44[8];
    s32 *f52;
    u32 f56;
    u8 pad60[8];
} Rec;

typedef struct {
    u8 pad0[204];
    Rec rec[16];
} Obj;

extern Obj class_w;
extern s16 basyo_init_tbl0;
extern s16 mes_base_y;
extern s32 basyo_init_tbl1;
extern s32 basyo_init_tbl2;
extern s32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;

extern void class_clear_n();
extern void set_kanji_string();

void basyo_init(arg0)
    s32 arg0;
{
    s32 i;

    mes_base_y = -64;
    for (i = 0; i < basyo_init_tbl0 + 2; i++) {
        class_clear_n(i + 3);
    }
    set_kanji_string(-206, -152, 0, arg0, 0);
    basyo_init_tbl0 = strlen(arg0) / 2;
    for (i = 0; i < basyo_init_tbl0 + 2; i++) {
        class_clear_n(i + 3);
        class_w.rec[i].f1 = 2;
        class_w.rec[i].f2 = 64;
        class_w.rec[i].f56 = 0x40000000;
        class_w.rec[i].f3 = 164;
        class_w.rec[i].f12 = &basyo_init_tbl1;
        class_w.rec[i].f16 = &basyo_init_tbl2;
        class_w.rec[i].f52 = &_sys_default_tpage_set_tbl2;
        class_w.rec[i].f20 = basyo_init_tbl3;
        class_w.rec[i].f4 = 8;
        class_w.rec[i].f5 = 1;
        class_w.rec[i].f24 = 114;
        class_w.rec[i].f38 = (basyo_init_tbl0 - i) * 14 + mes_base_y - 135;
        class_w.rec[i].f42 = mes_base_y - 81;
    }
    class_w.rec[basyo_init_tbl0 + 1].f24 = 113;
    class_w.rec[basyo_init_tbl0 + 1].f38 += 2;
    class_w.rec[0].f24 = 115;
    class_w.rec[0].f38 -= 2;
}
