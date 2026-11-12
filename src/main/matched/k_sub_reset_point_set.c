/* Address: 0x8004EEC0 */
/* Size: 20 bytes (5 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s16 disp_string_state;
extern s16 k_sub_reset_point;

void k_sub_reset_point_set(void)
{
    k_sub_reset_point = disp_string_state;
}
