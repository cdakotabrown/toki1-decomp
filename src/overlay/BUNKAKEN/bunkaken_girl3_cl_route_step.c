/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t9:t0 0x90:rd:t8:t9 0x94:rs:t8:t9 0x9c:rt:t9:t0 0xd0:rt:t0:t1 0xd4:rs:t0:t1 0xd4:rt:t0:t1 0xd8:rt:t2:t4 0xdc:rd:t1:t3 0xdc:rt:t0:t1 0xe0:rs:t1:t3 0xe8:rt:t2:t4 0x11c:rt:t3:t5 0x120:rs:t3:t5 0x120:rt:t3:t5 0x124:rt:t5:t8 0x128:rd:t4:t7 0x128:rt:t3:t5 0x12c:rs:t4:t7 0x134:rt:t5:t8 */
/* @SymbolStart: 0x801428E0 */
/* @SymbolSize: 368 */
/* Address: 0x801428E0 (prologue 0x801428F4) */
/* Size: 368 bytes (92 insns) -- 2-state dispatcher (state=4,5) with */
#include "types.h"

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);
extern s32 get_g_zyotai_low(s32);

extern u8 now_year;
extern s32 girl3_flag;
extern s32 bunka_bukatu;
extern s32 bunka_girl;

void bunkaken_girl3_cl_route_step(void) {
    u8 val;
    s32 state;

    state = bunka_bukatu;
    bunka_girl = 3;
    switch (state) {
    case 4:
        val = now_year;
        switch (val) {
        case 95:
            if (get_g_zyotai_low(3) == 4 && (girl3_flag << 29) >= 0) {
                bunka_girl = 255;
                goto_sub_step(5);
            } else {
                goto_sub_step(54);
            }
            break;
        case 96:
            if (get_g_zyotai_low(3) == 4 && (girl3_flag << 29) >= 0) {
                bunka_girl = 255;
                goto_sub_step(5);
            } else {
                goto_sub_step(55);
            }
            break;
        case 97:
            if (get_g_zyotai_low(3) == 4 && (girl3_flag << 29) >= 0) {
                bunka_girl = 255;
                goto_sub_step(5);
            } else {
                goto_sub_step(56);
            }
            break;
        }
        break;
    case 5:
        goto_sub_step(57);
        break;
    default:
        game_reset();
        break;
    }
}
