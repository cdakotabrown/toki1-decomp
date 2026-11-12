/* Function: mes_window_switch */
/* Address: 0x8006606C */
/* Size: 456 bytes (114 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80066064 */
/* @SymbolSize: 464 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1a4 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 class_w[];
extern volatile s16 basyo_init_tbl0;
extern s16 mes_base_y;
extern u8  cw3_disp;
extern void k_disp_switch(s32, s32);

void mes_window_switch(s32 a0)
{
    s32 mode;
    s32 i;

    mode = basyo_init_tbl0;

    if (mode != 0 || mode < 11) {
        if (a0 == 1) {
            for (i = 0; i < mode + 2; i++) {
                class_w[i * 68 + 207] |= 0x80;
            }
            k_disp_switch(0, 1);
        } else {
            if (cw3_disp & 0x80) {
                k_disp_switch(0, 0);
                mode = basyo_init_tbl0;
            }
            for (i = 0; i < mode + 2; i++) {
                class_w[i * 68 + 207] &= 0x7F;
            }
            mes_base_y = 184;
        }
    }
}
