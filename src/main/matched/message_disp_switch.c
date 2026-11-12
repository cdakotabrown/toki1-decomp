/* Address: 0x80064FE8, Size: 60 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 mes_win_cw_disp;
void message_disp_switch(s32 a) {
    if (a == 1) {
        mes_win_cw_disp = (u8)(mes_win_cw_disp | 0x80);
    } else {
        mes_win_cw_disp = (u8)(mes_win_cw_disp & 0x7F);
    }
}
