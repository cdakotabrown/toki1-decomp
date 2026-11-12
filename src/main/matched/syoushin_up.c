/* Address: 0x80070ED0 */
/* Size: 128 bytes (32 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers t9 t0 */
/* @PostProc: swap_registers t8 t9 */
/* @PostProc: swap_registers t7 t8 */
#include "types.h"

extern u8 screen_w;
extern s32 get_h_yuukou(s32);

void syoushin_up(void) {
    s32 i;
    u8 *p;

    p = &screen_w;
    i = 0;
    do {
        if ((*(s32 *)(p + 456) << 30) < 0) {
            if (get_h_yuukou(i) < 10) {
                *(s16 *)(p + 454) += 5;
            }
        }
        i++;
        p += 56;
    } while (i != 11);
}
