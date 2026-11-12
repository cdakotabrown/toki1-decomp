/* Address: 0x80132DB0 */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void strcpy(void *, void *);
extern u8 ba_string2[];
extern u8 shougatu_ba_string2_set_str0[];

void shougatu_ba_string2_set(void)
{
    strcpy(ba_string2, shougatu_ba_string2_set_str0);
    sub_sub_step_up();
}
