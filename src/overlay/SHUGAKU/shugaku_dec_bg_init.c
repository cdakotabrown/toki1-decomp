/* @SymbolStart: 0x80137F30 */
/* @SymbolSize: 140 */
/* @CompileFlags: -cckr */
#include "types.h"

extern s32 *mdec_bs;
extern u8 now_sub_sub_step;

extern void dec_bg_reset();
extern s32 sub_sub_step_up();
extern void dec_bg_init();
s32 shugaku_dec_bg_init()
{
    s32 *p = mdec_bs;

    if ((p[0] & 0xFFFF0000) != 0x38000000 || (p[1] & 0xFF010000) != 0x10000) {
        dec_bg_reset();
        sub_sub_step_up();
        now_sub_sub_step -= 3;
        return 0;
    }
    dec_bg_init(0);
    sub_sub_step_up();
}
