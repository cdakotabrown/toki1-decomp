/* Function: search_tpage_multi */
/* Address: 0x800489E4 */
/* Size: 388 bytes (97 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map "48:64" */
/* @PostProc: swap_instructions 0x1C 0x20 */
/* @PostProc: swap_instructions 0x80 0x84 */
/* @PostProc: swap_instructions 0xB4 0xB8 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void image_upload_queue_add_flags(s32 a0, s32 a1, u8 a2, s32 a3, s32 a4, s32 a5, s32 a6);

extern s32 screen_w[];

s32 search_tpage_multi(s32 arg0, u8 arg1) {
    s32 *base;
    s32 t0;
    s32 a0_val, v1_val;

    base = &screen_w[15];
    a0_val = 0x7FFE;
    t0 = 15;

    do {
        s32 *v0_ptr;
        a0_val = 48;
        v1_val = 192;
        v0_ptr = (s32 *)((s32)base + 192);

        do {
            if (v0_ptr[1098] == 0x7FFE) {
                if (v0_ptr[1162] == arg0) {
                    return (u16)(a0_val + t0);
                }
            }
            a0_val -= 16;
            v1_val -= 64;
            v0_ptr = (s32 *)((s32)v0_ptr - 64);
        } while (v1_val >= 0);

        t0--;
        base--;
    } while (t0 >= 5);

    t0 = 0;

    do {
        if (screen_w[t0 + 1098] == arg0) {
            return (u16)t0;
        }
        t0++;
    } while (t0 < 64);

    t0 = 0;

    do {
        if (screen_w[t0 + 1098] == 0x7FFF) {
            screen_w[t0 + 1098] = arg0;
            if (t0 < 32) {
                image_upload_queue_add_flags(arg0, t0 & 0xFF, arg1, 0, 0, 128, 128);
            } else {
                image_upload_queue_add_flags(arg0, t0 & 0xFF, arg1, 0, 128, 128, 128);
            }
            return (u16)t0;
        }
        if (t0 == 63) {
            return 63;
        }
        t0++;
    } while (t0 != 64);
}
