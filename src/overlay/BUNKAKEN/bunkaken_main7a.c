/* @SymbolStart: 0x80150D70 */
/* @RodataBase: 0x80155B30 */
/* @SymbolSize: 344 */
/* @PostProc: swap_registers t7:v0,t8:v0,t9:v0,v1:v0 */
/* Address: 0x80150D78 */
/* Size: 336 bytes (84 insns + cross-prologue trailing thunk to bunkaken_main7b) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_main7a(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        val = now_year;
        switch (val) {
        case 96: goto_sub_step(123); break;
        case 97: goto_sub_step(124); break;
        default: game_reset(); break;
        }
        break;
    case 1:
        goto_sub_step(125);
        break;
    case 2:
        goto_sub_step(126);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 96: goto_sub_step(127); break;
        case 97: goto_sub_step(126); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 96: goto_sub_step(128); break;
        case 97: goto_sub_step(129); break;
        }
        break;
    case 5:
        goto_sub_step(130);
        break;
    default:
        game_reset();
        break;
    }
}
