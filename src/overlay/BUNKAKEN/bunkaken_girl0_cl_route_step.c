/* @SymbolStart: 0x8014A6E0 */
/* @SymbolSize: 776 */
/* @RodataBase: 0x80155260 */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: patch_insn_reg_field 0x90:rt:t9:t0 0x94:rd:t8:t9 0x98:rs:t8:t9 0xa0:rt:t9:t0 0xd4:rt:t0:t1 0xd8:rs:t0:t1 0xd8:rt:t0:t1 0xdc:rt:t2:t4 0xe0:rd:t1:t3 0xe0:rt:t0:t1 0xe4:rs:t1:t3 0xec:rt:t2:t4 0x150:rt:t3:t5 0x154:rs:t3:t5 0x154:rt:t3:t5 0x158:rt:t5:t8 0x15c:rd:t4:t7 0x15c:rt:t3:t5 0x160:rs:t4:t7 0x168:rt:t5:t8 0x1f0:rt:t6:t9 0x1f4:rs:t6:t9 0x1f4:rt:t6:t9 0x1f8:rt:t8:t2 0x1fc:rd:t7:t1 0x1fc:rt:t6:t9 0x200:rs:t7:t1 0x208:rt:t8:t2 0x258:rt:t9:t3 0x25c:rs:t9:t3 0x25c:rt:t9:t3 0x260:rt:t1:t6 0x264:rd:t0:t5 0x264:rt:t9:t3 0x268:rs:t0:t5 0x270:rt:t1:t6 0x2b4:rt:t2:t7 0x2b8:rs:t2:t7 0x2b8:rt:t2:t7 0x2bc:rt:t4:t0 0x2c0:rd:t3:t9 0x2c0:rt:t2:t7 0x2c4:rs:t3:t9 0x2cc:rt:t4:t0 */
/* Address: 0x8014A6E0 (prologue 0x8014A6F0) */
/* Size: 776 bytes (194 insns) */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 get_g_zyotai_low(s32);

extern u8 now_year;
extern s32 girl0_flag;
extern s32 bunka_bukatu;
extern s32 bunka_girl;

void bunkaken_girl0_cl_route_step(void) {
    u8 val;

    bunka_girl = 0;
    switch (bunka_bukatu) {
    case 0:
        val = now_year;
        switch (val) {
        case 95:
            if ((get_g_zyotai_low(0) == 4) && ((girl0_flag << 29) >= 0)) {
                bunka_girl = 255;
                goto_sub_step(5);
            } else {
                goto_sub_step(33);
            }
            break;
        case 96:
            if ((get_g_zyotai_low(0) == 4) && ((girl0_flag << 29) >= 0)) {
                bunka_girl = 255;
                goto_sub_step(5);
            } else {
                goto_sub_step(33);
            }
            break;
        case 97:
            goto_sub_step(34);
            break;
        default:
            game_reset();
            break;
        }
        break;
    case 1:
        goto_sub_step(35);
        break;
    case 2:
        if ((get_g_zyotai_low(0) == 4) && ((girl0_flag << 29) >= 0)) {
            bunka_girl = 255;
            goto_sub_step(5);
        } else {
            goto_sub_step(36);
        }
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95:
            goto_sub_step(37);
            break;
        case 96:
            goto_sub_step(38);
            break;
        case 97:
            goto_sub_step(39);
            break;
        }
        if ((get_g_zyotai_low(0) == 4) && ((girl0_flag << 29) >= 0)) {
            bunka_girl = 255;
            goto_sub_step(5);
        }
        break;
    case 4:
        val = now_year;
        switch (val) {
        case 95:
            if ((get_g_zyotai_low(0) == 4) && ((girl0_flag << 29) >= 0)) {
                bunka_girl = 255;
                goto_sub_step(5);
            } else {
                goto_sub_step(40);
            }
            break;
        case 96:
            goto_sub_step(41);
            break;
        case 97:
            if ((get_g_zyotai_low(0) == 4) && ((girl0_flag << 29) >= 0)) {
                bunka_girl = 255;
                goto_sub_step(5);
            } else {
                goto_sub_step(42);
            }
            break;
        }
        break;
    case 5:
        goto_sub_step(43);
        break;
    default:
        game_reset();
        break;
    }
}
