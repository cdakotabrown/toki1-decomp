/* Address: 0x80043B8C */
/* Size: 128 bytes (32 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x0:rt:v0:v1 --patch 0x4:rt:v0:v1 --patch 0x4:rs:v0:v1 --patch 0xc:rs:v0:v1 --patch 0x24:rt:v0:v1 */
/* @PostProc: swap_instructions 0x2c 0x30 */
/* @PostProc: swap_instructions 0x30 0x3c */
/* @PostProc: swap_instructions 0x34 0x40 */
/* @PostProc: swap_instructions 0x38 0x44 */
/* @PostProc: swap_instructions 0x3c 0x48 */
/* @PostProc: swap_instructions 0x40 0x4c */
/* @PostProc: swap_instructions 0x44 0x50 */
/* @PostProc: swap_instructions 0x48 0x54 */
/* @PostProc: swap_instructions 0x4c 0x54 */
/* @PostProc: swap_instructions 0x50 0x54 */
#include "types.h"

extern u8 screen_w[];
extern u8 image_queue_n;

s32 image_upload_queue_add(u32 w, u8 b4, u16 h8, u16 h10, u16 h12, u16 h14) {
    u32 entry;

    entry = image_queue_n;
    if (entry >= 32) {
        goto full;
    }

    entry = (u32)&screen_w[entry << 4];
    *(u16 *)(entry + 5174) = h10;
    *(u16 *)(entry + 5176) = h12;
    *(u16 *)(entry + 5178) = h14;
    *(u32 *)(entry + 5164) = w;
    *(u8 *)(entry + 5168) = b4;
    *(u8 *)(entry + 5169) = 0;
    *(u8 *)(entry + 5170) = 0;
    *(u8 *)(entry + 5171) = 0;
    *(u16 *)(entry + 5172) = h8;

    image_queue_n = image_queue_n + 1;
    return 1;
full:
    return 0;
}
