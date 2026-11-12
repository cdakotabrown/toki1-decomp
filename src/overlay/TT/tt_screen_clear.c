/* tt_screen_clear - TT overlay, Address: 0x8013E810, Size: 32 bytes (8 insns) */
#include "types.h"

extern void screen_clear(void);

void tt_screen_clear(void)
{
    screen_clear();
}
