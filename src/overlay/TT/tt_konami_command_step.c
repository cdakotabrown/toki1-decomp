/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=0 IDO52_AS1_XBB_VOLATILE_OK=0 */
/* @SymbolStart: 0x8013EC5C */
/* @SymbolSize: 268 */
/* tt_konami_command_step - TT overlay, Address: 0x8013EC64 (true entry 0x8013EC5C) */
#include "types.h"

extern u8 *tt_step_wk;
extern u8 *tt_game_wk;
extern u8 *volatile tt_game_wk_v;
extern u16 tt_konami_cmd_tbl[];

extern s32 tt_cmd_once(s32 soundId);
extern s32 tt_trail_group_init(void);

void tt_konami_command_step(void)
{
    s32 unused;
    u8 *p;

    p = tt_step_wk;
    if ((*(u16 *)(tt_game_wk + 20) & tt_konami_cmd_tbl[p[33]]) == tt_konami_cmd_tbl[p[33]]) {
        p[32] = 16;
        p[33]++;
        if (p[33] == 10) {
            if (*(u16 *)(p + 0) == 2 && *(u16 *)(p + 2) >= 80U) {
                tt_cmd_once(2058);
                p[33] = 0;
            } else {
                tt_cmd_once(1802);
                p[33] = 0;
            }
            tt_trail_group_init();
        }
    } else if (*(u16 *)(tt_game_wk_v + 20) != 0) {
        p[33] = 0;
    }
    p[32]--;
    if (p[32] == 0) {
        p[33] = 0;
    }
}
