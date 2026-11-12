/* @SymbolStart: 0x8014E480 */
/* @RodataBase: 0x80155760 */
/* @SymbolSize: 416 */
/* @PostProc: swap_registers t7:v0,t8:v0,t9:v0,v1:v0 */
/* Address: 0x8014E488 */
/* Size: 408 bytes (102 insns + cross-prologue trailing thunk to bunkaken_girl7_pair_bk0_95_exh_main) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_girl7_pair_route_step(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(99); break;
        case 96: goto_sub_step(100); break;
        case 97: goto_sub_step(101); break;
        default: game_reset(); break;
        }
        break;
    case 1:
        goto_sub_step(102);
        break;
    case 2:
        goto_sub_step(103);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(104); break;
        case 96: goto_sub_step(105); break;
        case 97: goto_sub_step(106); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(107); break;
        case 96: goto_sub_step(108); break;
        case 97: goto_sub_step(109); break;
        }
        break;
    case 5:
        goto_sub_step(110);
        break;
    default:
        game_reset();
        break;
    }
}
