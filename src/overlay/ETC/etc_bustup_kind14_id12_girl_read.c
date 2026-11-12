/* @SymbolStart: 0x8013BBA8 */
/* @SymbolSize: 68 */
/* Address: 0x8013BBB8 (cross-prologue head at 0x8013BBA8 absorbed) */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern void bustup_cd_read(s32);
extern s32 etc_bustup_cd5_tbl[];
extern s32 etc_bustup_flag;

void etc_bustup_kind14_id12_girl_read(s32 a0)
{
    s32 a1 = etc_bustup_cd5_tbl[a0];

    if (a1 == -1) {
        return;
    }
    etc_bustup_flag = 1;
    bustup_cd_read(a1);
}
