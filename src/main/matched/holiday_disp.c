/* Address: 0x80075C9C */
/* Size: 60 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void holiday_icon_main(void);
extern void basyo_show(void);
extern void icon_cursor_move(s32);

void holiday_disp(void) {
    if (now_sub_sub_step == 0) {
        holiday_icon_main();
    }

    basyo_show();
    icon_cursor_move(0);
}
