/* Address: 0x80043C0C */
/* Size: 132 bytes (33 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions 0x20 0x24 */
/* @PostProc: swap_instructions 0x24 0x28 */
/* @PostProc: swap_instructions 0x28 0x2c */
/* @PostProc: swap_instructions 0x30 0x34 */
/* @PostProc: swap_instructions 0x34 0x40 */
/* @PostProc: swap_instructions 0x38 0x44 */
/* @PostProc: swap_instructions 0x3c 0x48 */
/* @PostProc: swap_instructions 0x40 0x4c */
/* @PostProc: swap_instructions 0x44 0x50 */
/* @PostProc: swap_instructions 0x48 0x54 */
/* @PostProc: swap_instructions 0x4c 0x58 */
/* @PostProc: swap_instructions 0x54 0x58 */
#include "types.h"

extern u8 screen_w[];

s32 image_upload_queue_add_flags(u32 a0, u8 a1, u8 a2, u16 a3, u16 s0, u16 s1, u16 s2) {
    u32 entry;

    entry = screen_w[0x1429];
    if (entry >= 32) {
        goto full;
    }

    entry = (u32)&screen_w[entry << 4];
    *(u16 *)(entry + 0x1436) = s0;
    *(u16 *)(entry + 0x1438) = s1;
    *(u16 *)(entry + 0x143A) = s2;
    *(u32 *)(entry + 0x142C) = a0;
    *(u8 *)(entry + 0x1430) = a1;
    *(u8 *)(entry + 0x1431) = a2;
    *(u8 *)(entry + 0x1432) = 0;
    *(u8 *)(entry + 0x1433) = 0;
    *(u16 *)(entry + 0x1434) = a3;

    screen_w[0x1429] = screen_w[0x1429] + 1;
    return 1;
full:
    return 0;
}
