/* @SymbolStart: 0x801436AC */
/* @SymbolSize: 308 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=63:t0,64:t1,65:a3 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* tt_scene_init - TT overlay, Address: 0x801436B4 (window 0x801436AC: the */
#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_cel_buf;
extern u8 tt_anim_tbl[];

extern void tt_player_config_set(void);

void tt_scene_init(void)
{
    u8 *p;
    u8 *v0;
    s32 v1;

    p = tt_player_wk;
    *(s8 *)(p + 0) = 0;
    *(s16 *)(p + 2) = 0;
    *(s8 *)(p + 12) = 0;
    *(s16 *)(p + 4) = 0;
    *(s8 *)(p + 13) = 0;
    *(s8 *)(p + 14) = 0;
    *(s8 *)(p + 15) = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 16) = 2;
    *(u8 **)(p + 56) = tt_anim_tbl;
    *(s8 *)(p + 60) = 0;
    *(s8 *)(p + 61) = 0;
    *(s8 *)(p + 62) = 0;
    *(s8 *)(p + 64) = 96;
    *(s8 *)(p + 72) = 0;
    tt_player_config_set();
    *(s8 *)(p + 87) = 0;
    *(s8 *)(p + 88) = 0;
    *(s8 *)(p + 89) = 0;
    *(s8 *)(p + 90) = 0;
    *(s8 *)(p + 91) = 0;
    *(s8 *)(p + 92) = 0;
    *(s8 *)(p + 93) = 0;
    *(s8 *)(p + 94) = 0;
    *(s8 *)(p + 95) = 0;
    *(s8 *)(p + 96) = 0;
    *(s16 *)(p + 4) = 32;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 6) = 0;
    *(s8 *)(p + 15) = 0;
    *(s8 *)(p + 14) = 0;
    *(s8 *)(p + 13) = 0;

    v0 = tt_cel_buf;
    v1 = 5;
    do {
        *(s8 *)(v0 + 62) = v1;
        v1 += 5;
        v0 += 96;
        *(s8 *)(v0 - 96) = 0;
        *(s16 *)(v0 - 94) = 0;
        *(s16 *)(v0 - 92) = 0;
        *(s8 *)(v0 - 84) = 0;
        *(s16 *)(v0 - 86) = 0;
        *(s16 *)(v0 - 88) = 0;
        *(s16 *)(v0 - 90) = 0;
        *(s8 *)(v0 - 81) = 0;
        *(s8 *)(v0 - 82) = 0;
        *(s8 *)(v0 - 83) = 0;
        *(s16 *)(v0 - 80) = 2;
        *(u8 **)(v0 - 40) = tt_anim_tbl;
        *(s8 *)(v0 - 36) = 0;
        *(s8 *)(v0 - 35) = 0;
        *(s8 *)(v0 - 32) = 96;
    } while (v1 != 15);
}
