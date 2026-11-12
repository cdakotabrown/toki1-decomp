/* @SymbolStart: 0x80149060 */
/* @SymbolSize: 44 */
#include "types.h"

extern void _name_ent_mainc();
extern u8 now_sub_sub_step;

void name_ent_mainc(void)
{
    if (now_sub_sub_step == 0) {
        _name_ent_mainc();
    }
}
