/* Address: 0x80139D70 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void shougatu_date_serifu_run();
extern u8 now_girl;

void shougatu_girl1_serifu_step(void)
{
    now_girl = 1;
    shougatu_date_serifu_run();
}
