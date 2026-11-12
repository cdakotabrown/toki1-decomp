/* Address: 0x80043B20 */
/* Size: 108 bytes (27 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_registers v1:t0 --bidirectional */
/* @PostProc: swap_registers v1:t1 --bidirectional */
/* @PostProc: swap_registers v1:t2 --bidirectional */
/* @PostProc: swap_registers v1:t3 --bidirectional */
/* @PostProc: swap_registers v1:t4 --bidirectional */
/* @PostProc: swap_instructions 0x2c 0x3c */
/* @PostProc: swap_instructions 0x30 0x3c */
/* @PostProc: swap_instructions 0x34 0x3c */
/* @PostProc: swap_instructions 0x38 0x3c */
#include "types.h"

extern u8 screen_w[];
extern u8 palette_queue_n;

s32 palette_upload_enqueue(u32 w, u8 b4, u8 b5, u8 b6, u8 b7) {
    u32 entry;

    entry = palette_queue_n;
    if (entry >= 32) {
        goto full;
    }

    entry = (u32)&screen_w[entry << 3];
    *(u32 *)(entry + 4904) = w;
    *(u8 *)(entry + 4908) = b4;
    *(u8 *)(entry + 4909) = b5;
    *(u8 *)(entry + 4910) = b6;
    *(u8 *)(entry + 4911) = b7;

    palette_queue_n = palette_queue_n + 1;
    return 1;
full:
    return 0;
}
