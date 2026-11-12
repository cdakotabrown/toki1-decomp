/* Size: 72 bytes */
/* @SymbolSize: 72 */
#include "types.h"
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
void tt_timer(void)
{
    u8 *a;
    u8 *b;
    a = tt_step_wk;
    b = tt_scroll_wk;
    if (a[18] != 0) {
        if (*(u16 *)(b + 8) < 0x8CA0U) {
            *(u16 *)(b + 8) = *(u16 *)(b + 8) + 1;
        }
    }
}
