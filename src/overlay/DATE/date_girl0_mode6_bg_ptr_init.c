/* @SymbolSize: 112 */
/* Function: date_girl0_mode6_bg_ptr_init */
/* Overlay: DATE */
/* Address: 0x8014F2F0 */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern volatile s32 date_girl0_mode6_bg;
extern volatile s32 date_girl0_mode6_bg_1;
extern volatile s32 date_girl0_mode6_bg_2;
extern volatile s32 date_girl0_mode6_bg_3;
extern volatile s32 date_girl0_mode6_bg_4;
extern volatile s32 date_girl0_mode6_bg_5;
extern volatile s32 date_girl0_mode6_bg_6;

void date_girl0_mode6_bg_ptr_init(void) {
    date_girl0_mode6_bg = (s32)0x801B0000;
    date_girl0_mode6_bg_1 = (s32)0x801B2000;
    date_girl0_mode6_bg_2 = (s32)0x801B6000;
    date_girl0_mode6_bg_3 = (s32)0x801BA000;
    date_girl0_mode6_bg_4 = (s32)0x801BE000;
    date_girl0_mode6_bg_5 = (s32)0x801C2000;
    date_girl0_mode6_bg_6 = (s32)0x801C6000;
}
