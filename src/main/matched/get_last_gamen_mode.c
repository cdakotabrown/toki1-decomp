/* Address: 0x80057704 */
/* Size: 16 bytes (to the next known symbol at 0x80057714) */
/* Address: 0x80057704 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 cd_wait_timeout_tbl0;

s32 get_last_gamen_mode(void)
{
    return cd_wait_timeout_tbl0;
}
