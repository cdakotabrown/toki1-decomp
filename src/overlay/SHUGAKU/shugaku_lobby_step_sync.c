/* @SymbolStart: 0x801322F0 */
/* @SymbolSize: 116 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 */
#include "types.h"

extern u8 now_sub_sub_step;
extern volatile u8 basho_num;

extern void shugaku_lobby_bg_set(void);
extern void indexed_byte_toggle(s32 a0);

void shugaku_lobby_step_sync(void)
{
    u32 lim;
    s32 saved;

    saved = now_sub_sub_step;
    shugaku_lobby_bg_set();
    lim = now_sub_sub_step;

    if ((saved + 3) == (s32)lim) {
        if (basho_num == 0) {
            return;
        }
    }
    if ((saved + 4) != (s32)lim) {
        return;
    }
    if (basho_num != 0) {
        return;
    }
    now_sub_sub_step--;
    indexed_byte_toggle(1);
}
