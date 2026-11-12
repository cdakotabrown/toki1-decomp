/* @SymbolSize: 176 */
/* sparkle_make - TT overlay, Address: 0x80134874 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=60:a1 */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 *tt_shot_buf;
extern u8 *tt_boss_wk;
extern TT_BOX tt_hit_box_tbl[];

extern u8 *get_tt_shot_free(u8 *a0, u8 *a1);

void sparkle_make(s16 a0arg, s16 a1arg)
{
    u8 *a0 = tt_shot_buf;
    u8 *t6 = tt_boss_wk;
    u8 *v0;

    v0 = get_tt_shot_free(a0, a0 + 13312);
    if (v0 == 0) {
        return;
    }
    *(s8 *)(v0 + 0) = 1;
    *(s16 *)(v0 + 2) = 173;
    *(s16 *)(v0 + 4) = 32;
    *(s16 *)(v0 + 10) = 0;
    *(s16 *)(v0 + 8) = 0;
    *(s16 *)(v0 + 6) = 0;
    *(s8 *)(v0 + 15) = 0;
    *(s8 *)(v0 + 14) = 0;
    *(s8 *)(v0 + 13) = 0;
    *(s16 *)(v0 + 16) = 1;
    *(s32 *)(v0 + 32) = a0arg << 16;
    *(s32 *)(v0 + 36) = a1arg << 16;
    *(s32 *)(v0 + 40) = 0;
    *(s32 *)(v0 + 44) = 0;
    *(s16 *)(v0 + 96) = *(s16 *)(t6 + 20);
    *(s16 *)(v0 + 88) = 0;
    *(s32 *)(v0 + 76) = (s32)((u8 *)&tt_hit_box_tbl[5].x);
}
