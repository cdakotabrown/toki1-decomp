/* @SymbolStart: 0x801361B4 */
/* @SymbolSize: 568 */
/* @RodataBase: 0x8013CBF4 */
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
typedef struct {
    u8  mode;
    u8  pad1[4];
    u8  fade;
    u8  pad6[431];
    u8  bits;
} Sys;
extern Sys csr_speed;
extern u8  step_mode;

option_csr_speed_main()
{
    s32 i;
    s32 s;
    s32 j;

    class_w[106].f3 = 128;
    menu_check(1, class_w[0].f38, class_w[0].f42);
    menu_cursor_move(1);
    option_menu_box_show();
    s = menu_csr;
    for (i = 0, j = 0; i < 5; i++, j += 2) {
        if (&cw96_active[s] == &cw96_active[i]) {
            cw96_active[i + 11].f24 = j + 11;
        } else {
            cw96_active[i + 11].f24 = j + 12;
        }
    }
    if (pad_trig & 0x20) {
        switch (s) {
        case 0:
            csr_speed.mode = 1;
            csr_speed.fade = 255;
            csr_speed.bits |= 8;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 1:
            csr_speed.mode = 2;
            csr_speed.fade = 255;
            csr_speed.bits |= 8;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 2:
            csr_speed.mode = 3;
            csr_speed.fade = 255;
            csr_speed.bits |= 8;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 3:
            csr_speed.mode = 4;
            csr_speed.fade = 255;
            csr_speed.bits |= 8;
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 4:
            csr_speed.mode = 6;
            csr_speed.fade = 255;
            csr_speed.bits |= 8;
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
