/* Address: 0x8006167C */
/* Size: 128 bytes (32 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
#include "types.h"

extern void uwasa_init(void);
extern void uwasa0(void);
extern void icon_cursor_move(s32);
extern void func_8006486C(void);
extern void func_80064F8C(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void dec_bg_show(s32);
extern void func_80066D38(s32);

void func_8006167C(s32 mode) {
    switch (mode) {
        case 0:
            uwasa_init();
            break;
        case 1:
            uwasa0();
            break;
    }
    icon_cursor_move(1);
    func_8006486C();
    func_80064F8C();
    message_window_show();
    basyo_show();
    dec_bg_show(0);
    func_80066D38(1);
}
