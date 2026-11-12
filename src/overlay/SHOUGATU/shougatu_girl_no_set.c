/* @SymbolStart: 0x80134360 */
/* @SymbolSize: 44 */
/* Address: 0x80134368 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern u8 girl_no;
extern u8 now_girl;

void shougatu_girl_no_set(void)
{
    girl_no = now_girl;
    sub_sub_step_up();
}
