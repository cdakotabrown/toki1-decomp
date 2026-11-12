/* @SymbolStart: 0x8013BCFC */
/* @SymbolSize: 68 */
/* Address: 0x8013BD0C (cross-prologue head at 0x8013BCFC absorbed) */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern void bustup_cd_read(s32);
extern s32 etc_bustup_cd10_tbl[];
extern s32 etc_bustup_flag;

void etc_bustup_kind4_girl_read(s32 a0)
{
    s32 a1 = etc_bustup_cd10_tbl[a0];

    if (a1 == -1) {
        return;
    }
    etc_bustup_flag = 1;
    bustup_cd_read(a1);
}
