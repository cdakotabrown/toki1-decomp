/* valen_girl9_serifu_step - VALEN overlay, Address: 0x801327E8, Size: 40 bytes (10 insns) */
#include "types.h"

extern void valen_serifu_run(void);
extern void date_name_set(s32);

void valen_girl9_serifu_step(void)
{
    date_name_set(9);
    valen_serifu_run();
}
