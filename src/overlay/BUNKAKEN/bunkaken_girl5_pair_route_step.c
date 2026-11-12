/* @SymbolStart: 0x80135060 */
/* @RodataBase: 0x80153440 */
/* @SymbolSize: 416 */
/* @PostProc: swap_registers t7:v0,t8:v0,t9:v0,v1:v0 */
/* Address: 0x80135068 */
/* Size: 408 bytes (102 insns + cross-prologue trailing thunk to bunkaken_girl5_pair_bk0_95_exh_main) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_girl5_pair_route_step(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(73); break;
        case 96: goto_sub_step(74); break;
        case 97: goto_sub_step(75); break;
        default: game_reset(); break;
        }
        break;
    case 1:
        goto_sub_step(76);
        break;
    case 2:
        goto_sub_step(77);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(78); break;
        case 96: goto_sub_step(79); break;
        case 97: goto_sub_step(80); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(81); break;
        case 96: goto_sub_step(82); break;
        case 97: goto_sub_step(83); break;
        }
        break;
    case 5:
        goto_sub_step(84);
        break;
    default:
        game_reset();
        break;
    }
}
