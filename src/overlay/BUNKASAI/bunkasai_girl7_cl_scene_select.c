/* @SymbolSize: 372 */
/* @RodataBase: 0x8015D7F0 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x34:rt:v0:t7 0x54:rt:t9:t8 0x5c:rs:t9:t8 0xa4:rt:t0:v0 0xa8:rs:t0:v0 0xa8:rt:v1:v0 0xb0:rs:v1:v0 0xb8:rs:v1:v0 0xc0:rs:v1:v0 0x100:rt:t1:v0 0x104:rs:t1:v0 0x104:rt:v1:v0 0x10c:rs:v1:v0 0x114:rs:v1:v0 0x11c:rs:v1:v0 */
/* Address: 0x80143784 */
/* Size: 352 bytes (88 insns) */
#include "types.h"

extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

extern void goto_sub_step(s32 a0);
extern void game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_girl7_cl_scene_select(void) {
    s32 sel;
    u8 val;

    bunka_girl = 7;
    sel = bunka_bukatu;

    switch (sel) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(123);
        } else {
            goto_sub_step(122);
        }
        break;
    case 1:
        goto_sub_step(124);
        break;
    case 2:
        goto_sub_step(125);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(126); break;
        case 96: goto_sub_step(128); break;
        case 97: goto_sub_step(125); break;
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(129); break;
        case 96: goto_sub_step(130); break;
        case 97: goto_sub_step(131); break;
        }
        break;
    case 5:
        goto_sub_step(132);
        break;
    default:
        game_reset();
        break;
    }
}
