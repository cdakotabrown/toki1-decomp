/* @SymbolStart: 0x801459D4 */
/* @SymbolSize: 44 */
/* Address: 0x801459DC */
/* Size: 36 bytes (9 instructions) */
#include "types.h"

extern void update_anim_state();

extern s32 step_state;

void save_file_anim_disp(void)
{
    s32 t6 = step_state;

    if (t6 == 0) {
        return;
    }
    update_anim_state(0);
}
