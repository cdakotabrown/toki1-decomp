/* Function: holiday_init */
/* Address: 0x80075528 */
/* Size: 88 bytes (22 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void holiday_screen_init(void);
extern void holiday_bg_init(void);
extern void basyo_show(void);
extern void icon_cursor_move(s32);

void holiday_init(void) {
    s32 mode = now_sub_sub_step;
    switch (mode) {
    default:
    case 0:
        holiday_screen_init();
        break;
    case 1:
        holiday_bg_init();
        break;
    }
    basyo_show();
    icon_cursor_move(0);
}
