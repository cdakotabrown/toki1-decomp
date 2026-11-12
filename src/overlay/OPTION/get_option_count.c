/* get_option_count - OPTION overlay, Address: 0x80136EF4, Size: 260 bytes (65 insns) */
/* @SymbolSize: 260 */
#include "types.h"

extern u8 album_work[];
extern u8 screen_w[];

s32 get_option_count(void)
{
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 14; i++) {
        if (album_work[0x14 + i] != 0 || screen_w[0x53C + i] != 0) {
            count++;
        }
    }
    return count;
}
