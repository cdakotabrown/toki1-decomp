/* @SymbolSize: 292 */
/* @RodataBase: 0x8015D240 */
/* Address: 0x8013FDA4 */
/* Size: 272 bytes (68 insns) */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x34:rt:v0:t7 0x54:rt:t9:t8 0x5c:rs:t9:t8 0xa4:rt:t0:t9 0xa8:rs:t0:t9 0xa8:rt:t1:t9 0xb0:rs:t1:t9 0xd8:rt:t2:t0 0xdc:rs:t2:t0 0xdc:rt:t3:t0 0xe4:rs:t3:t0 */
#include "types.h"

extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

extern void goto_sub_step(s32 a0);
extern void game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_main9a(void) {
    s32 sel;

    bunka_girl = 10;
    sel = bunka_bukatu;
    switch (sel) {
    case 0:
        if (now_year == 96) {
            goto_sub_step(155);
        } else {
            goto_sub_step(156);
        }
        break;
    case 1:
        goto_sub_step(157);
        break;
    case 2:
        goto_sub_step(158);
        break;
    case 3:
        if (now_year == 96) {
            goto_sub_step(159);
        } else {
            goto_sub_step(158);
        }
        break;
    case 4:
        if (now_year == 96) {
            goto_sub_step(160);
        } else {
            goto_sub_step(161);
        }
        break;
    case 5:
        goto_sub_step(162);
        break;
    default:
        game_reset();
        break;
    }
}
