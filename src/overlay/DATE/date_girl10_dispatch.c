/* @SymbolStart: 0x80159760 */
/* @SymbolSize: 148 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
#include "types.h"

extern s32 game_reset(void);
extern s32 date_girl10_mode3_dispatch(void);
extern s32 date_girl10_mode4_dispatch(void);
extern s32 date_girl10_mode5_dispatch(void);
extern s32 date_girl10_mode6_dispatch(void);

extern s32 work_tarao;

s32 date_girl10_dispatch(void)
{
    switch (work_tarao) {
    case 3:
        return date_girl10_mode3_dispatch();
    case 4:
        return date_girl10_mode4_dispatch();
    case 5:
        return date_girl10_mode5_dispatch();
    case 6:
        return date_girl10_mode6_dispatch();
    default:
        return game_reset();
    }
}
