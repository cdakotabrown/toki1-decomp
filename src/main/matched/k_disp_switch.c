/* Address: 0x8004ED64 */
/* Size: 52 bytes (13 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 k_disp_switch_tbl0[];

void k_disp_switch(s32 idx, s32 n) {
    if (n > 0) {
        k_disp_switch_tbl0[idx * 8] = 1;
    } else {
        k_disp_switch_tbl0[idx * 8] = 0;
    }
}
