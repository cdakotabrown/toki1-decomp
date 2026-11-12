/* Address: 0x8007C738 */
/* Size: 152 bytes (38 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 152 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u16 bustup_addr_set3_tbl0[];
extern u8 *bustup_clut;
extern u8 *bustup_img;
extern u8 *bustup_img2;
extern u8 *bustup_img3;

void bustup_addr_set3(s32 a0) {
    s32 idx;
    u16 *row;

    bustup_clut = (u8 *)0x801E0000;
    idx = a0 % 3;
    row = &bustup_addr_set3_tbl0[idx * 3];
    bustup_img3 = (u8 *)0x801E0000 + row[0];
    bustup_img = (u8 *)0x801E0000 + row[1];
    bustup_img2 = (u8 *)0x801E0000 + row[2];
}
