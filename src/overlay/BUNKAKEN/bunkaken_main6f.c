/* @SymbolStart: 0x8013EB30 */
/* @RodataBase: 0x80154280 */
/* @SymbolSize: 360 */
/* @PostProc: swap_registers t8:v0,t9:v0,v1:v0 */
/* @PostProc: patch_insn_reg_field 0x48:rt:v0:t7 0x50:rs:v0:t7 */
/* Address: 0x8013EB38 */
/* Size: 352 bytes (88 insns + cross-prologue trailing thunk to bunkaken_girl8_pair_bk0_9596_exh) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_main6f(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        val = now_year;
        if (val == 97) {
            goto_sub_step(113);
        } else {
            goto_sub_step(112);
        }
        break;
    case 1:
        goto_sub_step(114);
        break;
    case 2:
        goto_sub_step(115);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(116); break;
        case 96: goto_sub_step(117); break;
        case 97: goto_sub_step(115); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(118); break;
        case 96: goto_sub_step(119); break;
        case 97: goto_sub_step(120); break;
        }
        break;
    case 5:
        goto_sub_step(121);
        break;
    default:
        game_reset();
        break;
    }
}
