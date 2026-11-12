/* Address: 0x80140F50, Size: 68 bytes (17 instructions) */
#include "types.h"

extern u8 pad_type;
extern u8 mouse_btn_mode;
extern u8 get_etc_yes_button_str0[];
extern u8 etc_migi_str[];
extern u8 etc_hidari_str[];

u8 *get_etc_yes_button(void)
{
    if (pad_type == 0) {
        if (mouse_btn_mode != 0) {
            return etc_migi_str;
        }
        return etc_hidari_str;
    }
    return get_etc_yes_button_str0;
}
