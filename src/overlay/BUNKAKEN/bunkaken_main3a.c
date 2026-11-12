/* @SymbolStart: 0x80137640 */
/* @RodataBase: 0x801537A0 */
/* @SymbolSize: 452 */
/* @PostProc: swap_registers t7:v0,t8:v0,t9:v0,v1:v0 --exclude-offsets 0x188,0x18c,0x194 */
/* @PostProc: swap_registers t0:t7 --exclude-offsets 0x1a0 */
/* @PostProc: patch_insn_reg_field 0x18c:rt:v0:t7 0x194:rs:v0:t7 */
/* Address: 0x80137648 */
/* Size: 444 bytes (111 insns + cross-prologue trailing thunk to bunkaken_main3b) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_main3a(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(59); break;
        case 96: goto_sub_step(60); break;
        case 97: goto_sub_step(61); break;
        default: game_reset(); break;
        }
        break;
    case 1:
        goto_sub_step(62);
        break;
    case 2:
        goto_sub_step(63);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(64); break;
        case 96: goto_sub_step(65); break;
        case 97: goto_sub_step(66); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(67); break;
        case 96: goto_sub_step(68); break;
        case 97: goto_sub_step(69); break;
        }
        break;
    case 5:
        val = now_year;
        if (val == 97) {
            goto_sub_step(71);
        } else {
            goto_sub_step(70);
        }
        break;
    default:
        game_reset();
        break;
    }
}
