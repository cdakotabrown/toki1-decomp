/* Address: 0x80043AFC */
/* Size: 20 bytes (5 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 36 windows only this function's own bytes. */
/* @SymbolSize: 36 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 back_clear_flag;

void back_clear_switch(s32 a0)
{
    if (a0 != 0) {
        back_clear_flag = 1;
    } else {
        back_clear_flag = 0;
    }
}
