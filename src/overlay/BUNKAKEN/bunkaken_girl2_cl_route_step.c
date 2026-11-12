/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: patch_insn_reg_field 0x60:rt:t9:t0 0x64:rd:t8:t9 0x68:rs:t8:t9 0x70:rt:t9:t0 0xf4:rt:t0:t1 0xf8:rs:t0:t1 0xf8:rt:t0:t1 0xfc:rt:t2:t4 0x100:rd:t1:t3 0x100:rt:t0:t1 0x104:rs:t1:t3 0x10c:rt:t2:t4 */
/* @SymbolStart: 0x80140F70 */
/* @SymbolSize: 296 */
/* Address: 0x80140F70 (prologue 0x80140F84) */
/* Size: 296 bytes (74 insns) -- 3-state dispatcher with bit-2 sign */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 get_g_zyotai_low(s32);

extern u8  now_year;
extern s32 girl2_flag;
extern s32 bunka_bukatu;
extern s32 bunka_girl;

void bunkaken_girl2_cl_route_step(void) {
    u8 val;
    s32 state;

    state = bunka_bukatu;
    bunka_girl = 2;
    switch (state) {
    case 2:
        if (get_g_zyotai_low(2) == 4 && (girl2_flag << 29) >= 0) {
            bunka_girl = 255;
            goto_sub_step(5);
        } else {
            goto_sub_step(49);
        }
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(50); break;
        case 96: goto_sub_step(51); break;
        case 97: goto_sub_step(52); break;
        }
        if (get_g_zyotai_low(2) == 4 && (girl2_flag << 29) >= 0) {
            bunka_girl = 255;
            goto_sub_step(5);
        }
        break;
    default:
        game_reset();
        break;
    }
}
