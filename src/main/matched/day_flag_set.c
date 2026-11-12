/* Function: day_flag_set */
/* Address: 0x8007265C */
/* Size: 20 bytes (5 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 day_flag;
extern u8 day_cnt;

void day_flag_set(u8 a0)
{
    day_flag = a0;
    day_cnt = a0;
}
