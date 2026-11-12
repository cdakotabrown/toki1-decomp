/* @SymbolSize: 248 */
/* @PostProc: patch_insn_reg_field 0xd0:rd:t6:t7 0xd8:rt:t7:t6 */
#include "types.h"

extern s32 rsin(s32);

extern u8 *tt_boss_wk;
extern u8 *tt_player_wk;

void tt_sway_step(void)
{
    u8 *v1;
    u8 *a1;
    s32 v0, a0;

    v1 = tt_boss_wk;
    a1 = tt_player_wk;
    v0 = *(s32 *)(a1 + 32) - *(s32 *)(v1 + 996);
    if (v0 < 0) {
        a0 = 0 - v0;
    } else {
        a0 = v0;
    }
    if (a0 < 16384) {
        *(s32 *)(v1 + 996) = *(s32 *)(a1 + 32);
    } else if (v0 < 0) {
        *(s32 *)(v1 + 996) = *(s32 *)(v1 + 996) - 16384;
    } else {
        *(s32 *)(v1 + 996) = *(s32 *)(v1 + 996) + 16384;
    }
    v0 = *(s32 *)(v1 + 996);
    if (v0 < (s32)0xFFC00000) {
        *(s32 *)(v1 + 996) = (s32)0xFFC00000;
    } else if (v0 < 0x00400001) {

    } else {
        *(s32 *)(v1 + 996) = 0x00400000;
    }
    a0 = *(s32 *)(v1 + 1000);
    v0 = rsin(a0);
    *(s32 *)(v1 + 32) = (v0 << 9) + *(s32 *)(v1 + 996);
    *(s32 *)(v1 + 1000) = *(s32 *)(v1 + 1000) + 8;
    if (*(s32 *)(v1 + 1000) < 4097) {
        return;
    }
    *(s32 *)(v1 + 1000) = 0;
}
