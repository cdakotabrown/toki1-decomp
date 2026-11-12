/* Address: 0x8007C5F0 */
/* Size: 108 bytes (27 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u16 bust_up_ptr_set_tbl0[];
extern u8 *bustup_clut;
extern u8 *bustup_img;
extern u8 *bustup_img2;
extern u8 *bustup_img3;

void bust_up_ptr_set(s32 arg0) {
    u32 idx;
    u16 *row;

    bustup_clut = (u8 *)0x801E0000;
    idx = (u32)(arg0 % 25);
    row = &bust_up_ptr_set_tbl0[idx * 3];
    bustup_img3 = (u8 *)0x801E0000 + row[0];
    bustup_img = (u8 *)0x801E0000 + row[1];
    bustup_img2 = (u8 *)0x801E0000 + row[2];
}
