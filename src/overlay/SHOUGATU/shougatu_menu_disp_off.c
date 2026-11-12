/* @SymbolSize: 96 */
/* Address: 0x801400A0 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 class_w[];
extern u8 shougatu_pair_disp_on;
extern u8 shougatu_phase_on;

void shougatu_menu_disp_off(void)
{
    s16 v0;

    for (v0 = 0; v0 < 6; v0++) {
        class_w[v0 * 68 + 6531] = 0;
    }
    shougatu_pair_disp_on = 0;
    shougatu_phase_on = 0;
    sub_sub_step_up();
}
