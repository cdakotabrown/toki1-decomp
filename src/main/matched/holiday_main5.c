/* Function: holiday_main5 */
/* Address: 0x80076FD4 */
/* Size: 148 bytes (37 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u32 step_counter;
extern u8 now_sub_sub_step;
extern void holiday_girl_init(void);
extern void holiday_bustup_init(void);
extern void holiday_girl_exit(void);
extern void parameter_show(void);
extern void basyo_show(void);
extern void icon_cursor_move(s32);

void holiday_main5(void) {
    step_counter++;
    {
        s32 mode = now_sub_sub_step;
        switch (mode) {
        case 0:
            holiday_girl_init();
            break;
        case 1:
            holiday_bustup_init();
            break;
        case 2:
            holiday_girl_exit();
            break;
        }
    }
    parameter_show();
    basyo_show();
    icon_cursor_move(1);
}
