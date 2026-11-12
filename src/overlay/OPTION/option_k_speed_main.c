/* @SymbolStart: 0x80133994 */
/* @SymbolSize: 396 */
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

extern Rec class_w[];
extern Rec cw96_active[];

extern s8  menu_csr;
extern s32 pad_trig;
extern u8  option_on;
extern u8  step_mode;

option_k_speed_main()
{
    s32 i;
    s32 s;
    s32 j;

    class_w[106].f3 = 128;
    menu_check(1, class_w[0].f38, class_w[0].f42);
    menu_cursor_move(1);
    option_menu_box_show();
    s = menu_csr;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (&cw96_active[s] == &cw96_active[i]) {
            cw96_active[i + 11].f24 = j + 13;
        } else {
            cw96_active[i + 11].f24 = j + 14;
        }
    }
    if (pad_trig & 0x20) {
        switch (s * 1) {
        case 0:
            k_speed_set(0);
            option_on |= 0x80;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 1:
            k_speed_set(1);
            option_on |= 0x80;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 2:
            k_speed_set(3);
            option_on |= 0x80;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        }
    }
    if (pad_trig & 0x40) {
        sub_sub_step_up();
        step_mode = 2;
    }
}
