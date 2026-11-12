/* @SymbolStart: 0x8014686C */
/* @SymbolSize: 168 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=128:1:1 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* tt_fade_out80 - TT overlay, Address: 0x8014686C (absorbed head), 168 bytes. */
/* @PostProc: patch_insn_reg_field 0x7c:rt:v0:t8 0x80:rt:t8:t0 0x84:rs:v0:t8 0x84:rt:v0:t9 0x88:rs:v0:t9 0x8c:rt:v0:t9 0x9c:rt:t8:t0 */
#include "types.h"

extern u8 *tt_step_wk;
extern s16 *tt_text_buf;
extern s16 *tt_anim_buf;

extern void tt_board_scale_step(void);

void tt_fade_out80(void)
{
    u8 *a2 = tt_step_wk;
    s16 *q;
    s32 i;
    s32 v;
    s32 dec;

    if ((u8)a2[29] == 128) {
        i = 0;
        q = tt_text_buf;
        do {
            i += 1;
            q = (s16 *)((u8 *)q + 20);
            *(s16 *)((u8 *)q - 14) = 8;
        } while (i < 128);

        q = tt_anim_buf;
        i = 0;
        do {
            i += 4;
            *(s16 *)((u8 *)q + 168) = 8;
            *(s16 *)((u8 *)q + 268) = 8;
            *(s16 *)((u8 *)q + 368) = 8;
            q = (s16 *)((u8 *)q + 400);
            *(s16 *)((u8 *)q - 332) = 8;
        } while (i != 128);
    }

    dec = a2[29] - 4;
    v = dec & 0xFF;
    a2[29] = (u8)dec;
    a2[31] = (u8)v;
    a2[30] = (u8)v;
    if (v == 0) {
        a2[9] = 1;
    }
    tt_board_scale_step();
}
