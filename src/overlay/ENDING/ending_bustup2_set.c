/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* ending_bustup2_set - ENDING overlay, Address: 0x80137180, Size: 480 bytes (120 instructions) */
#include "types.h"

extern void class_clear_n(s32);

extern u8 class_w[];
extern s32 wink2_stop;
extern u8 bustup2_cw_prio;
extern s16 bustup2_cw_anim;
extern u8 now_girl;

s32 ending_bustup2_set(s32 a0, s32 a1, s32 a2, s16 a3)
{
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s16 s4;
    u8 *s5;
    s32 s6;
    u32 s7;
    s32 fp;
    u8 *entry;
    s32 v0;
    s32 stride;

    s1 = a1;
    s2 = a2;
    s3 = a0;
    s4 = a3;

    if (a1 == 0) {
        class_clear_n(30);
        class_clear_n(31);
        return 0;
    }

    s5 = class_w;
    s0 = 0;
    fp = 10;
    s7 = 0x41000000U;
    s6 = 68;

    do {
        class_clear_n(s0 + 30);

        stride = s0 * s6;
        entry = s5 + stride;

        *(u8 *)(entry + 2041) = (u8)fp;
        *(u8 *)(entry + 2042) = 0;
        *(u32 *)(entry + 2096) = s7;
        *(u8 *)(entry + 2043) = 132;
        *(s32 *)(entry + 2052) = s1;
        *(s32 *)(entry + 2056) = s2;
        *(s32 *)(entry + 2092) = s3;
        *(s16 *)(entry + 2060) = s4;
        *(s16 *)(entry + 2062) = 0;
        *(s16 *)(entry + 2064) = 0;
        *(u8 *)(entry + 2046) = 2;
        *(u8 *)(entry + 2107) = 19;
        *(s16 *)(entry + 2078) = -160;
        *(s16 *)(entry + 2082) = -392;
        *(u8 *)(entry + 2047) = 128;
        *(u8 *)(entry + 2045) = 0;
        *(u8 *)(entry + 2044) = 0;

        s0++;
        s0 = (s16)s0;
    } while (s0 < 2);

    wink2_stop = 0;
    bustup2_cw_prio = 9;
    bustup2_cw_anim = 7;

    v0 = now_girl;

    if (v0 == 1 || v0 == 3 || v0 == 12) {
        bustup2_cw_anim = 4;
    } else if (v0 == 2 || v0 == 8 || v0 == 11) {
        bustup2_cw_anim = 5;
    } else if (v0 == 4 || v0 == 10) {
        bustup2_cw_anim = 6;
    } else if (v0 == 9) {
        bustup2_cw_anim = 8;
    }
}
