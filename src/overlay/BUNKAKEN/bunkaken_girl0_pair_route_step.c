/* @SymbolStart: 0x801325B0 */
/* @RodataBase: 0x80153090 */
/* @SymbolSize: 452 */
/* @PostProc: swap_registers t7:v0,t8:v0,t9:v0,v1:v0 --exclude-offsets 0x188,0x18c,0x194 */
/* @PostProc: swap_registers t0:t7 --exclude-offsets 0x1a0 */
/* @PostProc: patch_insn_reg_field 0x18c:rt:v0:t7 0x194:rs:v0:t7 */
/* Address: 0x801325B8 */
/* Size: 452 bytes (113 insns including cross-prologue trailing thunk) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_girl0_pair_route_step(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(19); break;
        case 96: goto_sub_step(20); break;
        case 97: goto_sub_step(21); break;
        default: game_reset(); break;
        }
        break;
    case 1:
        goto_sub_step(22);
        break;
    case 2:
        goto_sub_step(23);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(24); break;
        case 96: goto_sub_step(25); break;
        case 97: goto_sub_step(26); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(27); break;
        case 96: goto_sub_step(28); break;
        case 97: goto_sub_step(29); break;
        }
        break;
    case 5:
        val = now_year;
        if (val == 97) {
            goto_sub_step(31);
        } else {
            goto_sub_step(30);
        }
        break;
    default:
        game_reset();
        break;
    }
}
