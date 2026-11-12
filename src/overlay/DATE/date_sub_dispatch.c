/* @SymbolStart: 0x8014A154 */
/* @SymbolSize: 156 */
/* @RodataBase: 0x8015B380 */
#include "types.h"

extern void game_reset(void);
extern void date_open_load_step(void);
extern void date_open_dispatch(void);
extern void date_cmd512_send(void);
extern void date_cdwait3(void);
extern void date_bustup_check(void);

extern u16 date_open_step;

void date_sub_dispatch(void)
{
    switch (date_open_step) {
        case 0: date_open_load_step(); break;
        case 1: date_open_dispatch(); break;
        case 2: date_cmd512_send(); break;
        case 3: date_cdwait3(); break;
        case 4: date_bustup_check(); break;
        default: game_reset(); break;
    }
}
