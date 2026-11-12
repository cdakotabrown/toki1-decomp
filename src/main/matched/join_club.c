/* Function: join_club */
/* Address: 0x80060764 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void join_club_init(void);
extern void join_club_select(void);
extern void join_club_message(void);
extern void join_club_exit(void);
extern void cal_base_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void step_directional_scroll_once(void);

void join_club(void) {
    s32 mode = now_sub_sub_step;
    switch (mode) {
    case 0:
        join_club_init();
        break;
    case 1:
        join_club_select();
        break;
    case 2:
        join_club_message();
        break;
    case 3:
        join_club_exit();
        break;
    }
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    step_directional_scroll_once();
}
