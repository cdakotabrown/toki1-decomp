/* @SymbolStart: 0x80148E24 */
/* @SymbolSize: 44 */
#include "types.h"

extern void _name_ent_mainb();
extern u8 now_sub_sub_step;

void name_ent_mainb(void)
{
    if (now_sub_sub_step == 0) {
        _name_ent_mainb();
    }
}
