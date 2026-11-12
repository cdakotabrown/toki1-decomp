/* @SymbolSize: 448 */
/* @RodataBase: 0x8015F740 */
/* @PostProc: swap_registers a0:t7 --exclude-offsets 0x54,0x64,0x74,0xb0,0xc0,0xd0,0x10c,0x11c,0x12c,0x168,0x178,0x188,0x198 */
/* @PostProc: swap_registers t6:t7 --unidirectional */
/* @PostProc: swap_registers t8:v0,t9:v0,v1:v0 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t7:t6 0x18:rt:t7:t6 0x1c:rt:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x34:rt:v0:t7 0x50:rt:v0:t8 0x54:rs:v0:t8 0x54:rt:v0:t8 0x5c:rs:v0:t8 0x68:rt:t7:a0 0x78:rt:t7:a0 0x88:rt:t7:a0 0xc4:rt:t7:a0 0xd4:rt:t7:a0 0xe4:rt:t7:a0 0xf0:rt:t0:v0 0xf4:rs:t0:v0 0x120:rt:t7:a0 0x130:rt:t7:a0 0x140:rt:t7:a0 0x14c:rt:t1:v0 0x150:rs:t1:v0 0x17c:rt:t7:a0 0x18c:rt:t7:a0 0x19c:rt:t7:a0 0x1ac:rt:t7:a0 */
/* Address: 0x8015402C */
/* Size: 428 bytes (107 insns + cross-prologue trailing thunk to bunkasai_main1d) */
#include "types.h"

extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_main1c(void) {
    s32 sel;
    u8 val;

    bunka_girl = 1;
    sel = bunka_bukatu;

    switch (sel) {
    case 0:
        val = now_year;
        if (val == 97) {
            goto_sub_step(30);
        } else {
            goto_sub_step(29);
        }
        break;
    case 1:
        goto_sub_step(31);
        break;
    case 2:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(32); break;
        case 96: goto_sub_step(33); break;
        case 97: goto_sub_step(34); break;
        }
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(35); break;
        case 96: goto_sub_step(37); break;
        case 97: goto_sub_step(34); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(38); break;
        case 96: goto_sub_step(39); break;
        case 97: goto_sub_step(40); break;
        }
        break;
    case 5:
        goto_sub_step(41);
        break;
    default:
        game_reset();
        break;
    }
}
