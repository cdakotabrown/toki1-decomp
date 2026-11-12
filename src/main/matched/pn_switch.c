/* Address: 0x8004B0D0 */
/* Size: 76 bytes (19 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers t8:v0 --unidirectional */
#include "types.h"

extern u8 _debug_number_show_tbl0;

void pn_switch(s32 a) {
    s32 v;

    if ((a & 0xFF) == 0) {
        v = 0;
    } else {
        v = (_debug_number_show_tbl0 - 1) & 0xFF;
    }
    _debug_number_show_tbl0 = v;
    if ((u32)v >= 9) {
        _debug_number_show_tbl0 = 8;
    }
}
