/* @SymbolStart: 0x80136D88 */
/* Address: 0x80136D90 */
/* Size: 144 bytes (36 insns) */
/* @SymbolSize: 144 */
#include "types.h"

extern s32 pad_data;

extern s32 taco_scroll_pos;

extern void sub_sub_step_up(void);
extern void taco_panel_disp(s32, s32);
extern void taco_drop(s32, s32);

void taco_scroll3(void)
{
    s32 arg0 = pad_data;

    if (arg0 & 0x40) {
        taco_scroll_pos = taco_scroll_pos + 120;
    } else {
        taco_scroll_pos = taco_scroll_pos + 60;
    }

    taco_drop(1, 0);
    taco_panel_disp(taco_scroll_pos, 3);

    if (taco_scroll_pos >= 4097) {
        sub_sub_step_up();
    }
}
