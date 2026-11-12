/* dec_bg_cache4045_step - ENDING overlay, Address: 0x80133410, Size: 40 bytes (10 instructions)  * Status: BYTE-PERFECT 🎉 */
#include "types.h"

extern void dec_bg_cache_step();
extern void sub_sub_step_up();

void dec_bg_cache4045_step(void)
{
    dec_bg_cache_step(16453);
    sub_sub_step_up();
}
