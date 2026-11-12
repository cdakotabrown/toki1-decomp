/* @SymbolStart: 0x80138A44 */
/* @SymbolSize: 104 */
/* Address: 0x80138A4C, Size: 96 bytes (24 instructions) */
#include "types.h"
extern void cd_read_start2();
extern s32 get_CD_State2();
extern void sub_step_up();
extern u8 step_mode;

void option_screen_load(void) {
    s32 hidden_t6;

    hidden_t6 = step_mode;
    if (hidden_t6 == 0) {
        cd_read_start2(58, 0x80180000, 0xA1AB);
        step_mode = step_mode + 1;
    } else {
        if (get_CD_State2() & 1) {
            sub_step_up();
        }
    }
}
