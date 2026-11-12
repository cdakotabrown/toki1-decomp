/* Address: 0x8006777C, Size: 96 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
extern u8 mascot_cw_disp;
extern u8 mascot2_cw_disp;
void mascot_disp_switch(s32 a) {
    if (a != 0) {
        mascot_cw_disp = (u8)(mascot_cw_disp | 0x80);
        mascot2_cw_disp = (u8)(mascot2_cw_disp | 0x80);
    } else {
        mascot_cw_disp = (u8)(mascot_cw_disp & 0x7F);
        mascot2_cw_disp = (u8)(mascot2_cw_disp & 0x7F);
    }
}
