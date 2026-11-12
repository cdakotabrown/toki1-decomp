/* @SymbolStart: 0x80151734 */
/* @SymbolSize: 148 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
#include "types.h"

extern void game_reset(void);
extern void date_girl3_mode2_dispatch(void);
extern void date_girl3_mode3_dispatch(void);
extern void date_girl3_mode4_dispatch(void);
extern void date_girl3_mode5_dispatch(void);

extern s32 work_tarao;

void date_girl3_dispatch(void)
{
    switch (work_tarao) {
    case 2:
        date_girl3_mode2_dispatch();
        break;
    case 3:
        date_girl3_mode3_dispatch();
        break;
    case 4:
        date_girl3_mode4_dispatch();
        break;
    case 5:
        date_girl3_mode5_dispatch();
        break;
    default:
        game_reset();
        break;
    }
}
