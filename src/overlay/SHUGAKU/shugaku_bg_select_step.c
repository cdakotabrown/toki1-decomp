/* @SymbolStart: 0x80135584 */
/* @SymbolSize: 60 */
#include "types.h"

extern u8 shugaku_rpg_arg;

extern void shugaku_lobby_bg_set(void);
extern void shugaku_bg_step(void);

void shugaku_bg_select_step(void)
{
    s32 t6_in = shugaku_rpg_arg;

    if (t6_in != 0) {
        shugaku_lobby_bg_set();
        return;
    }
    shugaku_bg_step();
}
