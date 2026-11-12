/* Function: date_don_text_set */
/* Overlay: DATE */
/* @SymbolSize: 100 */
#include "types.h"

extern void k_reset(s32);
extern void set_kanji_string(s32, s32, s32, u8*, s32);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

extern u8 date_don_text_set_str0[];

void date_don_text_set(void)
{
    k_reset(1);
    set_kanji_string(-128, 48, 1, date_don_text_set_str0, 0);
    device_cmd_send(0x22);
    device_cmd_send(0x21);
    device_cmd_send(0x2f);
    device_cmd_send(0x500);
    sub_sub_step_up();
}
