/* @SymbolStart: 0x8013BA80 */
/* @SymbolSize: 68 */
/* Address: 0x8013BA90 (cross-prologue head at 0x8013BA80 absorbed) */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern void bustup_cd_read(s32);
extern s32 etc_bustup_cd2_tbl[];
extern s32 etc_bustup_flag;

void etc_bustup_id34_girl_read(s32 a0)
{
    s32 a1 = etc_bustup_cd2_tbl[a0];

    if (a1 == -1) {
        return;
    }
    etc_bustup_flag = 1;
    bustup_cd_read(a1);
}
