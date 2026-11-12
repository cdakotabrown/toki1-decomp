/* @SymbolStart: 0x80148B30 */
/* @SymbolSize: 44 */
#include "types.h"

extern void _name_ent_maina();
extern u8 now_sub_sub_step;

void name_ent_maina(void)
{
    if (now_sub_sub_step == 0) {
        _name_ent_maina();
    }
}
