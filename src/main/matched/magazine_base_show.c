/* Likely Symbol: appraisal_base_show (from o.bin, offset +68 bytes from 0x80069230) */
/* Address: 0x80069274 - 0x800692C4 (80 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void _sprite_set_box_shade(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0, s32 stack1, s32 stack2);
extern void dtd_on(s32 a0);

void magazine_base_show(void) {
    _sprite_set_box_shade(-160, -120, 320, 158, 5, 0x00E0E0E0, 2);
    dtd_on(5);
}
