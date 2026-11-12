/* Address: 0x8004C50C */
/* Size: 572 bytes (143 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8004C50C.py */
#include "types.h"

extern void clut_gray_set(void *arg0);

void system_init(void *arg0, u32 *arg1, s32 rAdj, s32 gAdj, s32 bAdj)
{
    s32 rA;
    s32 gA;
    u8 *ptr;
    s32 count;
    u32 word;
    s32 r, g, b, v;
    u16 p2;

    rA = rAdj;
    gA = gAdj;
    clut_gray_set(arg0);

    ptr = (u8 *)arg1;
    count = 0;

    do {
        word = *(u32 *)ptr;
        count++;

        r = word & 0x1f;
        g = (word << 22) >> 27;
        b = (word << 17) >> 27;

        if (r + g + b != 0) {
            v = r + rA;
            if ((u32)v >= 32u) {
                ptr[0] |= 0x1f;
            } else {
                ptr[0] = (ptr[0] & 0xe0) | (v & 0x1f);
            }

            word = *(u32 *)ptr;
            g = (word << 22) >> 27;
            v = g + gA;
            if ((u32)v >= 32u) {
                *(u16 *)ptr |= 0x3e0;
            } else {
                *(u16 *)ptr = (*(u16 *)ptr & 0xfc1f) | ((v << 5) & 0x3e0);
            }

            word = *(u32 *)ptr;
            b = (word << 17) >> 27;
            v = b + bAdj;
            if ((u32)v >= 32u) {
                ptr[1] |= 0x7c;
                word = *(u32 *)ptr;
            } else {
                ptr[1] = (ptr[1] & 0x83) | ((v << 2) & 0x7c);
                word = *(u32 *)ptr;
            }
        }

        p2 = *(u16 *)(ptr + 2);
        r = p2 & 0x1f;
        g = (word << 6) >> 27;
        b = (word << 1) >> 27;

        if (r + g + b != 0) {
            v = r + rA;
            if ((u32)v >= 32u) {
                ptr[2] |= 0x1f;
            } else {
                ptr[2] = (ptr[2] & 0xe0) | (v & 0x1f);
            }

            word = *(u32 *)ptr;
            g = (word << 6) >> 27;
            v = g + gA;
            if ((u32)v >= 32u) {
                *(u16 *)(ptr + 2) |= 0x3e0;
            } else {
                *(u16 *)(ptr + 2) = (*(u16 *)(ptr + 2) & 0xfc1f) | ((v << 5) & 0x3e0);
            }

            word = *(u32 *)ptr;
            b = (word << 1) >> 27;
            v = b + bAdj;
            if ((u32)v >= 32u) {
                ptr[3] |= 0x7c;
            } else {
                ptr[3] = (ptr[3] & 0x83) | ((v << 2) & 0x7c);
            }
        }

        ptr += 4;
    } while (count != 128);
}
