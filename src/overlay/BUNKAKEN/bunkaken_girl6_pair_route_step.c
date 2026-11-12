/* @SymbolStart: 0x801465A0 */
/* @RodataBase: 0x80154CB0 */
/* @SymbolSize: 416 */
/* @PostProc: swap_registers t7:v0,t8:v0,t9:v0,v1:v0 */
/* Address: 0x801465A8 */
/* Size: 408 bytes (102 insns + cross-prologue trailing thunk to bunkaken_girl6_pair_bk0_95_exh_main) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_girl6_pair_route_step(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(86); break;
        case 96: goto_sub_step(87); break;
        case 97: goto_sub_step(88); break;
        default: game_reset(); break;
        }
        break;
    case 1:
        goto_sub_step(89);
        break;
    case 2:
        goto_sub_step(90);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(91); break;
        case 96: goto_sub_step(92); break;
        case 97: goto_sub_step(93); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(94); break;
        case 96: goto_sub_step(95); break;
        case 97: goto_sub_step(96); break;
        }
        break;
    case 5:
        goto_sub_step(97);
        break;
    default:
        game_reset();
        break;
    }
}
