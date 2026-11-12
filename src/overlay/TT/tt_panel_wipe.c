/* tt_panel_wipe - TT overlay, Address: 0x80143968, Size: 204 bytes (51 insns) */
/* `@SymbolSize: 204` windows the real boundary (the sanctioned recovery) and */
/* @SymbolSize: 204 */
#include "types.h"

extern u8 *tt_scroll_wk;
extern u8 *tt_player_wk;

void tt_panel_wipe(void)
{
    u8 *p;
    u8 *q;

    p = tt_player_wk;
    q = tt_scroll_wk;
    switch ((u32)*(u16 *)(p + 6)) {
    case 0:
        p[82] = 1;
        *(u16 *)(p + 80) = *(u16 *)(p + 80) | 2;
        p[86] = 1;
        p[90] = 0;
        p[91] = 0;
        *(s16 *)(p + 6) = 64;
        break;
    case 64:
        *(s32 *)(p + 40) = 0;
        *(s32 *)(p + 44) = 0xFFFC0000;
        if (*(s32 *)(p + 36) < -1048576) {
            *(s32 *)(p + 44) = 0;
            *(s16 *)(p + 6) = 80;
            *(s16 *)(q + 16) = 0;
        }
        break;
    case 80:
        if (*(u16 *)(tt_scroll_wk + 16) >= 91U) {
            *(s16 *)(p + 6) = 0;
            p[13] = 1;
        }
        *(u16 *)(q + 16) = *(u16 *)(q + 16) + 1;
        break;
    }
}
