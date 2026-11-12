/* Function: date_light_effect */
/* Address: 0x8006B9A0 */
/* Size: 320 bytes (80 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 date_light_effect_tbl0[11];
extern s16 date_light_effect_tbl1[11];
extern void _sprite_set_light_effect1(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp16, s32 sp20, s32 sp24, u32 sp28, s32 sp32);
extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp16);

void date_light_effect(void) {
    u32 color;
    s32 i;

    color = 0xA0A0A0;

    _sprite_set_light_effect1(-288, -248, date_light_effect_tbl0[0], 60, 160, 60, 9, color, date_light_effect_tbl1[0]);

    for (i = 1; i < 9; i++) {
        _sprite_set_light_effect1(-288, -248, date_light_effect_tbl0[i], 60, 0, 180, 9, color, date_light_effect_tbl1[i]);
    }

    dtd_on_tpage(0, 0, 9, 1, 0);

    _sprite_set_light_effect1(-320, -280, -322, -321, -162, -121, 9, 0x808080, 0);

    dtd_on_tpage(0, 0, 9, 1, 0);
}
