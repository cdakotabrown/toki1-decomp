/* @SymbolStart: 0x80132148 */
/* @SymbolSize: 80 */
/* @PostProc: swap_registers v0:v1 */
/* Address: 0x80132150 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void pad_wait_sub_step();
extern u8 now_sub_sub_step;
extern void etc_hizuke_basyo_init();

void hizuke_basyo_step(void)
{
    s32 v1 = now_sub_sub_step;

    switch (v1) {
    case 0:
        etc_hizuke_basyo_init();
        break;
    case 1:
        pad_wait_sub_step();
        break;
    }
}
