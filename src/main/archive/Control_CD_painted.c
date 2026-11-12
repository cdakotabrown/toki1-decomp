/* Function: Control_CD - Control_CD */
/* Address: 0x80050FA0 */
/* Size: 164 bytes (41 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80050FA0.py */
#include "types.h"

extern void _sprite_set_box_shade(s16 x, s16 y, u8 w, u8 h, u32 p5, u32 p6, u32 p7);
extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, u32 sp16);

void Control_CD(s32 offset, s32 index, s32 param1, s32 param2,
                   s32 param3, u8 *table, u8 *table2, u32 color) {
    u32 p3 = (u32)param3 & 0xF;
    u32 p1 = (u32)param1 & 0x7;
    u32 p2 = (u32)param2 << 7;
    u8 *row;

    if (index < 0) goto done;
    if ((u32)index >= table2[4240]) goto done;

    row = table + (index << 4) + (offset << 3);

    _sprite_set_box_shade(
        *(s16 *)(row + 3984),
        *(s16 *)(row + 3986),
        *(u8 *)(row + 3988),
        *(u8 *)(row + 3989),
        p3,
        color & 0xFFFFFF,
        p2 | p1
    );
    dtd_on_tpage(0, 0, p3, 0, 1);
done:;
}
