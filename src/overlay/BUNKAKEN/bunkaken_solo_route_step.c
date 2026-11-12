/* @SymbolStart: 0x80139E50 */
/* @RodataBase: 0x80153B40 */
/* @PostProc: patch_insn_reg_field 0x48:rt:t8:t7 0x50:rs:t8:t7 0x98:rt:t9:v0 0x9c:rs:t9:v0 0xf4:rt:t0:v0 0xf8:rs:t0:v0 */
/* @SymbolSize: 360 */
/* Address: 0x80139E58 */
/* Size: 352 bytes (88 insns) */
#include "types.h"

extern s32 goto_sub_step();
extern s32 game_reset();
extern s32 bunka_bukatu;

#define now_year (*(u8 *)0x800EE79E)

void bunkaken_solo_route_step(void) {
    s32 t6 = bunka_bukatu;
    u8 val;

    switch (t6) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(7);
        } else {
            goto_sub_step(6);
        }
        return;
    case 1:
        goto_sub_step(8);
        return;
    case 2:
        goto_sub_step(9);
        return;
    case 3:
        switch (now_year) {
        case 95: goto_sub_step(10); return;
        case 96: goto_sub_step(12); return;
        case 97: goto_sub_step(13); return;
        }
        return;
    case 4:
        switch (now_year) {
        case 95: goto_sub_step(14); return;
        case 96: goto_sub_step(15); return;
        case 97: goto_sub_step(16); return;
        }
        return;
    case 5:
        goto_sub_step(17);
        return;
    default:
        game_reset();
        return;
    }
}
