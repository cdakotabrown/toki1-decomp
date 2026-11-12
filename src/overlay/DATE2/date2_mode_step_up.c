/* @SymbolStart: 0x80133878 */
/* @SymbolSize: 64 */
/* date2_mode_step_up - DATE2 overlay, Address: 0x80133880, Size: 56 bytes (14 insns) */
#include "types.h"

extern u8 date2_mode_no;

void sub_step_up(void);
void sub_sub_step_up(void);

void date2_mode_step_up(void)
{
    if (date2_mode_no == 2) {
        sub_step_up();
    } else {
        sub_sub_step_up();
    }
}
