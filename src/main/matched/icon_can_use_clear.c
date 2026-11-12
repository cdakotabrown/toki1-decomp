/* Debug Symbol: icon_can_use_clear (from o.bin, exact match) */
/* Address: 0x8005AF00 */
/* Size: 172 bytes (43 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void icon_can_use_set(s32 a0, s32 a1);
extern u8 icon_mode;
extern u16 basyo_pack;

void icon_can_use_clear(s32 arg0) {
    s32 i;

    icon_can_use_set(10, 0);
    icon_can_use_set(8, 0);
    icon_can_use_set(9, 0);

    if (icon_mode == 2) {
        icon_can_use_set(14, 0);
    }

    if (((u32)basyo_pack >> 12) == 11) {
        icon_can_use_set(4, 0);
    }

    if (arg0 != 0) {
        for (i = 0; i < 8; i++) {
            icon_can_use_set(i, 0);
        }
    }
}
