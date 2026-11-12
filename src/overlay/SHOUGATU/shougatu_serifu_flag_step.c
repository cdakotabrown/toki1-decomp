/* Address: 0x8013B0A4 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void shougatu_date_serifu_run();
extern s32 kuchi_stop;
extern s16 shougatu_date_now_serifu_in;

void shougatu_serifu_flag_step(void)
{
    shougatu_date_serifu_run();
    if (shougatu_date_now_serifu_in == 2) {
        kuchi_stop = 1;
    } else {
        kuchi_stop = 0;
    }
}
