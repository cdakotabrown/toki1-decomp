/* @SymbolStart: 0x80141CB8 */
/* @SymbolSize: 64 */
#include "types.h"

extern s32 taiiku_state;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern void k_reset(s32);

void taiiku_menu_next(void)
{
    taiiku_state = 9;
    now_serifu = 2;
    now_serifu_in = 0;
    k_reset(1);
}
