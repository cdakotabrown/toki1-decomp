/* @SymbolStart: 0x80141CF8 */
/* @SymbolSize: 64 */
#include "types.h"

extern s32 taiiku_state;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern void k_reset(s32);

void taiiku_menu_back(void)
{
    taiiku_state = 11;
    now_serifu = 1;
    now_serifu_in = 0;
    k_reset(1);
}
