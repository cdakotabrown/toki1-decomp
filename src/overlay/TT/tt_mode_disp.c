/* @SymbolStart: 0x8013E1E8 */
/* @RodataBase: 0x8014D610 */
/* @PostProc: patch_insn_reg_field 0x20:rt:v0:t7 0x28:rs:v0:t7 0x30:rt:v0:t7 */
/* @SymbolSize: 168 */
/* Address: 0x8013E1F0, Size: 156 bytes (39 insns) */
#include "types.h"

extern u8 *tt_step_wk;
extern void tt_fill_disp(void);
extern void tt_fade_disp(void);
extern void tt_scroll_disp(void);
extern void tt_scroll_tint_disp(void);
extern void tt_net_disp(void);
extern void tt_board_disp(void);

void tt_mode_disp(void)
{
    u8 *t6 = tt_step_wk;
    u8 mode;

    mode = t6[28];

    switch (mode) {
        case 0:
            tt_fill_disp();
            break;
        case 1:
            tt_fade_disp();
            break;
        case 2:
            tt_scroll_disp();
            break;
        case 3:
            tt_scroll_tint_disp();
            break;
        case 16:
        case 17:
            tt_net_disp();
            break;
        case 32:
        case 33:
            tt_board_disp();
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        default:
            break;
    }
}
