/* @SymbolStart: 0x80139E28 */
/* @SymbolSize: 188 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_NO_ENTRY_HOME_STORE=occ:1 IDO52_COPYPROP_VAR=4 */
/* Address: 0x80139E28 (cross-prologue head: lui/lw t6, mdec_bs) / */
#include "types.h"

extern void printf();
extern void decdct();
extern void sub_sub_step_up();
extern u8 ending_bs_decode_str0[];
extern u8 ending_bs_head_fmt[];
extern s32 *mdec_bs;
extern s32 date_effect_init_tbl0;
extern s32 date_effect_init_tbl1;
extern u8 now_sub_sub_step;

ending_bs_decode(a0, n)
s32 a0;
s32 n;
{
    s32 *cmd;

    cmd = mdec_bs;
    if ((cmd[0] & 0xFFFF0000) == 0x38000000) {
        n = 256;
        if ((cmd[1] & 0xFF010000) == 0x10000) {
            goto small;
        }
    }
    printf(ending_bs_decode_str0, n);
    printf(ending_bs_head_fmt, mdec_bs[0], mdec_bs[1]);
    date_effect_init_tbl0 = 0;
    date_effect_init_tbl1 = 0;
    sub_sub_step_up();
    now_sub_sub_step -= 3;
    return 0;
small:
    decdct(320, n, 320, 240);
    sub_sub_step_up();
}
