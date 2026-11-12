/* @SymbolStart: 0x8013CF30 */
/* @RodataBase: 0x80154020 */
/* @SymbolSize: 280 */
/* /* @PostProc: swap_registers a0:t6 */
/* @PostProc: swap_registers t8:t7 */
/* @PostProc: swap_registers t1:t8 */
/* @PostProc: swap_registers t2:t9 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x9c:rs:t9:t8 0x5c:rt:t6:a0 0x6c:rt:t6:a0 0x7c:rt:t6:a0 0x8c:rt:t6:a0 0xb0:rt:t6:a0 0xc0:rt:t6:a0 0xe4:rt:t6:a0 0xf4:rt:t6:a0 0x104:rt:t6:a0 */
/* @PostProc: patch_insn_reg_field 0x9c:rt:t0:t8 */
/* @PostProc: patch_insn_reg_field 0xa4:rs:t0:t8 */
/* @PostProc: patch_insn_reg_field 0xd0:rs:t8:t9 */
/* Address: 0x8013CF38 */
/* Size: 272 bytes (68 insns) */
#include "types.h"

extern s32 goto_sub_step();
extern s32 game_reset();
extern s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkaken_girl10_pair_route_step(void) {
    s32 t6 = bunka_bukatu;
    switch (t6) {
    case 0:
        if (now_year == 96) {
            goto_sub_step(132);
        } else {
            goto_sub_step(133);
        }
        break;
    case 1:
        goto_sub_step(134);
        break;
    case 2:
        goto_sub_step(135);
        break;
    case 3:
        if (now_year == 96) {
            goto_sub_step(136);
        } else {
            goto_sub_step(135);
        }
        break;
    case 4:
        if (now_year == 96) {
            goto_sub_step(137);
        } else {
            goto_sub_step(138);
        }
        break;
    case 5:
        goto_sub_step(139);
        break;
    default:
        game_reset();
        break;
    }
}
