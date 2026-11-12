/* @SymbolStart: 0x80134A10 */
/* @SymbolSize: 108 */
/* shugaku_wipe_step2 -- SHUGAKU overlay, Address: 0x80134A18, Size: 100 bytes (25 insns) */
#include "types.h"

extern u8 shugaku_use_sprite_fade;

extern u8 step_mode;
extern void sprite_fade_main(void);
extern void indexed_byte_toggle(s32 a0);
extern void wipe_main(void);

void shugaku_wipe_step2(void)
{
    s32 t6 = shugaku_use_sprite_fade;

    if (t6 != 0) {
        sprite_fade_main();
        return;
    }
    if (step_mode == 0) {
        indexed_byte_toggle(1);
        step_mode = step_mode + 1;
    }
    wipe_main();
}
