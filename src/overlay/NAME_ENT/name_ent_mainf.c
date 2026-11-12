/* @SymbolStart: 0x8014A508 */
/* @SymbolSize: 44 */
#include "types.h"

extern void _name_ent_mainf();
extern u8 now_sub_sub_step;

void name_ent_mainf(void)
{
    if (now_sub_sub_step == 0) {
        _name_ent_mainf();
    }
}
