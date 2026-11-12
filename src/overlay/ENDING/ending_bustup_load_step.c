/* @SymbolStart: 0x80133880 */
/* @SymbolSize: 72 */
/* ending_bustup_load_step - ENDING overlay, Address: 0x80133880, Size: 72 bytes (18 instructions) */
#include "types.h"

extern u8 girl_no;
extern void bustup_cd_read();
extern void sub_sub_step_up();

void ending_bustup_load_step(void)
{
    u8 step = girl_no;

    if (step == 14) {
        bustup_cd_read(27428);
    } else {
        bustup_cd_read(22568);
    }
    sub_sub_step_up();
}
