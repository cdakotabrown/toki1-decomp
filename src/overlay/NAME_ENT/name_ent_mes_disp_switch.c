/* @SymbolStart: 0x80144598 */
/* @SymbolSize: 216 */
/* name_ent_mes_disp_switch - NAME_ENT overlay, Address: 0x801445A0, Size: 208 bytes (52 insns) */
#include "types.h"

extern void k_disp_switch();
extern void _sprite_set_box_shade();
extern void dtd_on_tpage();
extern s32 pad_data;

void name_ent_mes_disp_switch(void)
{
    if (pad_data & 0x0010) {
        k_disp_switch(0, 1);
        k_disp_switch(1, 1);
        k_disp_switch(2, 1);
        k_disp_switch(3, 1);
        _sprite_set_box_shade(-156, 8, 288, 88, 4, 0x00808080, 131);
        dtd_on_tpage(0, 0, 4, 0, 0);
    } else {
        k_disp_switch(0, 0);
        k_disp_switch(1, 0);
        k_disp_switch(2, 0);
        k_disp_switch(3, 0);
    }
}
