/* @SymbolStart: 0x80155560 */
/* @SymbolSize: 148 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
#include "types.h"

extern s32 game_reset(void);
extern s32 date_girl5_main3(void);
extern s32 date_effect_main(void);
extern s32 date_girl5_main5(void);
extern s32 date_girl5_main6(void);

extern s32 work_tarao;

s32 date_mode_dispatch2(void)
{
    switch (work_tarao) {
    case 3:
        return date_girl5_main3();
    case 4:
        return date_effect_main();
    case 5:
        return date_girl5_main5();
    case 6:
        return date_girl5_main6();
    default:
        return game_reset();
    }
}
