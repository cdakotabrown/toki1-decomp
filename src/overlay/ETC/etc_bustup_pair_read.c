/* @SymbolStart: 0x8013BB08 */
/* @SymbolSize: 92 */
#include "types.h"

extern void bustup_cd_read(s32);
extern s32 etc_bustup_pair_tbl[][11];
extern s32 etc_bustup_flag;

void etc_bustup_pair_read(s32 a0, s32 a1)
{
    s32 a2 = etc_bustup_pair_tbl[a0][a1];

    if (a2 == -1) {
        return;
    }
    etc_bustup_flag = 1;
    bustup_cd_read(a2);
}
