/* @SymbolSize: 140 */
/* get_tel_call_girl - TEL overlay, Address: 0x8013C308, Size: 148 bytes (37 insns) */
#include "types.h"

extern u8 screen_w[];

extern s32 get_g_zyotai_s(s32);

s32 get_tel_call_girl(void)
{
    s32 i;
    u32 flags;
    u8 *base;
    s32 result;

    base = screen_w;
    i = 0;

    do {
        flags = *(u32 *)(base + 456);

        if ((flags << 30) >> 31) {

            if ((flags << 19) >> 31) {
                result = get_g_zyotai_s(i);
                if (result != 4) {
                    return i;
                }
            }
        }
        i++;
        base += 56;
    } while (i != 11);

    return 0;
}
