/* en_nichi_marker_ripple - EN_NICHI overlay, Address: 0x80135B4C, Size: 120 bytes (30 insns) */
/* @SymbolSize: 120 */
#include "types.h"

extern void en_nichi_ripple_set(s32);
extern u32 en_nichi_marker_timer[];

void en_nichi_marker_ripple(void)
{
    s32 s1;
    s32 s2;
    u32 v0;
    u32 *s0;

    s0 = en_nichi_marker_timer;
    s1 = 0;
    s2 = 10;

    do {
        v0 = *s0;
        if (v0 != 0) {
            *s0 = v0 + 1;
            en_nichi_ripple_set(s1);
            if (*(s32 *)s0 >= 81) {
                *s0 = 0;
            }
        }
        s1++;
        s0++;
    } while (s1 != s2);
}
