/* @SymbolStart: 0x80133510 */
/* @SymbolSize: 88 */
/* valen_girl9_skip_step - VALEN overlay, Address: 0x80133510, Size: 88 bytes (22 insns) */
#include "types.h"

extern u8 valen_girl9_flag;
extern u8 now_girl;
extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);

void valen_girl9_skip_step(void)
{
    u8 sel;

    sel = valen_girl9_flag;
    if (sel == 1) {
        if (now_girl == 9) {
            now_sub_sub_step += 6;
            return;
        }
    }
    sub_sub_step_up();
}
