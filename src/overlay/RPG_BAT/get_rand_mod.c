/* get_rand_mod - RPG_BAT overlay, Address: 0x8013E9A8, Size: 88 bytes (22 insns) */
#include "types.h"

extern s32 rand();

s32 get_rand_mod(s32 a0)
{
    if (a0 == 0) {
        a0 = 1;
    }
    return rand() % a0;
}
