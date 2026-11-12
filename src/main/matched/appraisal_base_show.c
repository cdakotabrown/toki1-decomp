/* Debug Symbol: appraisal_base_show (from o.bin, high match) */
/* Address: 0x80069224 */
/* Size: 80 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void _sprite_set_box_shade(s16 a0, s16 a1, s16 a2, s16 a3, s32 stack0, s32 stack1, s32 stack2);
extern void dtd_on(s32 a0);

void appraisal_base_show(void) {
    _sprite_set_box_shade(-160, -104, 283, 142, 5, 0x00E0E0E0, 2);
    dtd_on(5);
}
