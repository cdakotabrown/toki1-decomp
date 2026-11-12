/* @SymbolStart: 0x801399E0 */
/* @SymbolSize: 112 */
#include "types.h"

extern u8 option_reverb_flag;
extern u8 option_reverb_toggle_str0[];
extern void SD_FlagToggle();
extern void debug_string_show();
extern void debug_hex_show();
extern s32 pad_trig;

void option_reverb_toggle(void) {
    s32 hidden_t6;
    s32 t7;
    u8 t8;
    u8 t9;
    u8 a0_val;

    hidden_t6 = pad_trig;

    t7 = hidden_t6 & 0x0800;
    if (t7 != 0) {
        t8 = option_reverb_flag;
        t9 = (u8)(t8 + 1);
        option_reverb_flag = t9;
        SD_FlagToggle();
    }
    debug_string_show(option_reverb_toggle_str0, 120, 0);
    a0_val = option_reverb_flag;
    debug_hex_show((s32)(a0_val & 1), 120, 10);
}
