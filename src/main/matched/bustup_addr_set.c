/* Address: 0x8007C65C */
/* Size: 108 bytes (27 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 *bustup_clut;
extern u8 *bustup_img;
extern u8 *bustup_img2;
extern u8 *bustup_img3;
extern u16 bustup_addr_set_tbl0[];

void bustup_addr_set(s32 a0) {
    u8 *base = (u8 *)0x801E0000;
    u32 idx;
    u16 *p;

    bustup_clut = base;
    idx = (u32)(a0 % 13);
    p = &bustup_addr_set_tbl0[idx * 3];
    bustup_img3 = base + p[0];
    bustup_img = base + p[1];
    bustup_img2 = base + p[2];
}
