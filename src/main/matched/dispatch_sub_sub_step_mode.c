/* Function: dispatch_sub_sub_step_mode */
/* Address: 0x800789E0 */
/* Size: 136 bytes (34 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void join_club_init(void);
extern void join_club_select(void);
extern void join_club_message(void);
extern void holiday_club_join_exit(void);
extern void cal_base_show(void);

void dispatch_sub_sub_step_mode(void) {
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
        holiday_club_join_exit();
        break;
    }
    cal_base_show();
}
