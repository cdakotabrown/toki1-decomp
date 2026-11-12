/* @PostProc: patch_insn_reg_field   0x50:rt:t8:t7 0x58:rs:t8:t7 0xa0:rt:t9:v0 0xa4:rs:t9:v0 0xa4:rt:v1:v0 0xac:rs:v1:v0 0xb4:rs:v1:v0 0xbc:rs:v1:v0 0xfc:rt:t0:v0 0x100:rs:t0:v0 0x100:rt:v1:v0 0x108:rs:v1:v0 0x110:rs:v1:v0 0x118:rs:v1:v0 */
/* @SymbolStart: 0x80148040 */
/* @SymbolSize: 368 */
/* @RodataBase: 0x8015DFE0 */
/* Address: 0x80148050 */
/* Size: 352 bytes (88 insns) */
#include "types.h"
extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

extern void goto_sub_step(s32 a0);
extern void game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_mainf(void) {
    u8 val;

    bunka_girl = 0;
    switch (bunka_bukatu) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(17);
        } else {
            goto_sub_step(16);
        }
        break;
    case 1:
        goto_sub_step(18);
        break;
    case 2:
        goto_sub_step(19);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(20); break;
        case 96: goto_sub_step(22); break;
        case 97: goto_sub_step(23); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(24); break;
        case 96: goto_sub_step(25); break;
        case 97: goto_sub_step(26); break;
        }
        break;
    case 5:
        goto_sub_step(27);
        break;
    default:
        game_reset();
        break;
    }
}
