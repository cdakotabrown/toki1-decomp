/* @SymbolSize: 244 */
/* etc_girls_all_check - ETC overlay, Address: 0x801431B0, Size: 244 bytes (61 insns) */
#include "types.h"

extern u8 album_work[];
extern u8 screen_w[];

s32 etc_girls_all_check(void)
{
    s32 i;

    for (i = 0; i != 13; i++) {
        if (album_work[0x14 + i] == 0 && screen_w[0x53C + i] == 0) {
            return 0;
        }
    }
    return 1;
}
