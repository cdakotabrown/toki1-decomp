/* @SymbolStart: 0x80156850 */
/* @SymbolSize: 148 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
#include "types.h"

extern s32 game_reset(void);
extern s32 date_girl6_mode2_dispatch(void);
extern s32 date_girl6_mode3_dispatch(void);
extern s32 date_girl6_mode4_dispatch(void);
extern s32 date_girl6_mode5_dispatch(void);

extern s32 work_tarao;

s32 date_mode_dispatch(void)
{
    switch (work_tarao) {
    case 2:
        return date_girl6_mode2_dispatch();
    case 3:
        return date_girl6_mode3_dispatch();
    case 4:
        return date_girl6_mode4_dispatch();
    case 5:
        return date_girl6_mode5_dispatch();
    default:
        return game_reset();
    }
}
