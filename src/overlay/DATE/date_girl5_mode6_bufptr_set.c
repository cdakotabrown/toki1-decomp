#include "types.h"

extern u32 date_screen_setup2_bg[];

void date_girl5_mode6_bufptr_set(void)
{
    date_screen_setup2_bg[0] = 0x801B0000;
    date_screen_setup2_bg[1] = 0x801B2000;
    date_screen_setup2_bg[2] = 0x801B6000;
    date_screen_setup2_bg[3] = 0x801BA000;
    date_screen_setup2_bg[4] = 0x801BE000;
    date_screen_setup2_bg[5] = 0x801C2000;
    date_screen_setup2_bg[6] = 0x801C6000;
}
