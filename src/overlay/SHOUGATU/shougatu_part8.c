/* @SymbolStart: 0x80137BB0 */
/* @SymbolSize: 120 */
/* Address: 0x80137BB8 */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern void shougatu_step_dispatch(void);
extern void shougatu_step_dispatch2(void);
extern u8 shougatu_date_kind;

void shougatu_part8(void)
{
    switch (shougatu_date_kind) {
    case 0:
        shougatu_step_dispatch();
        break;
    case 1:
        shougatu_step_dispatch();
        break;
    case 2:
        shougatu_step_dispatch();
        break;
    default:
        shougatu_step_dispatch2();
        break;
    }
}
