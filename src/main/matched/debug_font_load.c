/* Function: debug_font_load */
/* Address: 0x8004B154 */
/* Size: 64 bytes (16 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 32 48 */
#include "types.h"

extern void load_image_rect(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp10);
extern u8 _debug_number_show_str0;

void debug_font_load(void) {
    u8 flag = _debug_number_show_str0;

    if (flag != 0) {
        load_image_rect(704, 480, 64, 32, 0x801F0020);
    }
}
