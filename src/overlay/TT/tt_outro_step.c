/* @SymbolStart: 0x80132C24 */
/* @SymbolSize: 168 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=128:-1:1:1 */
/* tt_outro_step - TT overlay, Address: 0x80132C2C, Size: 160 bytes (40 insns) */
#include "types.h"

extern void tt_board_scroll(void);
extern u8 *tt_step_wk;
extern s16 *tt_anim_buf;
extern s16 *tt_text_buf;

void tt_outro_step(void)
{
    u8 *arg2 = tt_step_wk;
    s32 v1;
    s16 *v0;

    if (arg2[29] == 128) {
        v1 = 0;
        v0 = tt_text_buf;
        do {
            v1++;
            v0 += 10;
            v0[-7] = 8;
        } while (v1 < 128);

        v1 = 0;
        v0 = tt_anim_buf;
        do {
            v1 += 4;
            v0[84] = 8;
            v0[134] = 8;
            v0[184] = 8;
            v0 += 200;
            v0[-166] = 8;
        } while (v1 != 128);
    }

    arg2[29] = *(volatile u8 *)(arg2 + 29) - 4;
    v1 = arg2[29];
    arg2[31] = v1;
    arg2[30] = v1;
    if (v1 == 0) {
        arg2[9] = 1;
    }
    tt_board_scroll();
}
