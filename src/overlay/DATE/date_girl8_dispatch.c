/* @SymbolStart: 0x8014CF70 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8015B440 */
#include "types.h"

extern void date_fn_dispatch();
extern void date_fn_dispatch2();
extern void date_girl8_mode4_step();
extern void date_girl8_mode5_step();
extern void date_girl8_mode6_step();
extern void game_reset();

extern s32 work_tarao;

void date_girl8_dispatch(void)
{
    switch (work_tarao) {
        case 2: date_fn_dispatch(); break;
        case 3: date_fn_dispatch2(); break;
        case 4: date_girl8_mode4_step(); break;
        case 5: date_girl8_mode5_step(); break;
        case 6: date_girl8_mode6_step(); break;
        default: game_reset(); break;
    }
}
