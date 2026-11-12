/* @SymbolSize: 52 */
/* bat_zyotai_check - RPG_BAT overlay, Address: 0x80156870, Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 get_g_zyotai_s();

s32 bat_zyotai_check(void)
{
    s32 result;
    u32 masked;

    result = get_g_zyotai_s(6);
    masked = (u32)(result & 0x7F);
    if (masked < 2) {
        return 1;
    }
    return 0;
}
