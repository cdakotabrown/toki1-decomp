/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolStart: 0x801493F0 */
/* @SymbolSize: 216 */
/* tt_shot_command_step - TT overlay, Address: 0x801493F8 (true entry 0x801493F0) */
#include "types.h"

extern u8 *tt_step_wk;
extern u8 *tt_game_wk;
extern u8 *volatile tt_game_wk_v;
extern u8 *tt_text_buf;
extern u16 tt_shot_cmd_tbl[];

extern s32 tt_cmd_once(s32 soundId);

void tt_shot_command_step(void)
{
    s32 unused;
    u8 *p;

    p = tt_step_wk;
    if ((*(u16 *)(tt_game_wk + 20) & tt_shot_cmd_tbl[p[33]]) == tt_shot_cmd_tbl[p[33]]) {
        p[32] = 16;
        p[33]++;
        if (p[33] == 6) {
            tt_cmd_once(1543);
            p[33] = 0;
            p[36] = 1;
            p[37] = 0;
            tt_text_buf[120] = 1;
        }
    } else if (*(u16 *)(tt_game_wk_v + 20) != 0) {
        p[33] = 0;
    }
    p[32]--;
    if (p[32] == 0) {
        p[33] = 0;
    }
}
