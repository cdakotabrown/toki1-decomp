/* @SymbolStart: 0x80134920 */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 ret_sub_step;
extern void shugaku_maina_dispatch(void);
extern void shugaku_maina_dispatch2(void);

void shugaku_maina(void)
{
    u8 sel = ret_sub_step;

    if (sel == 3) {
        shugaku_maina_dispatch();
        return;
    }
    shugaku_maina_dispatch2();
}
