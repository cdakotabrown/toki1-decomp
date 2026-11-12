/* Function: uwasa_main0 */
/* Address: 0x80060DB0 */
/* Size: 80 bytes (20 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 step_mode;
extern void xa_wait(void);
extern void sub_sub_step_up(void);

void uwasa_main0(void) {
    s32 mode = step_mode;
    switch (mode) {
    case 0:
        xa_wait();
        break;
    default:
    case 1:
        sub_sub_step_up();
        break;
    }
}
