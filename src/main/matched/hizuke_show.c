/* Address: 0x80064F94 */
/* Size: 84 bytes (21 instructions) */
/* @SymbolStart: 0x80064F8C */
/* @SymbolSize: 92 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 cw16_disp;
extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on(s32);

void hizuke_show(void) {
    if (cw16_disp & 0x80) {
        _sprite_set_box_shade(134, -75, 20, 114, 5, 200192, 0);
        dtd_on(5);
    }
}
