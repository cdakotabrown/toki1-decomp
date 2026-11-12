/* @SymbolStart: 0x8013CA88 */
/* @SymbolSize: 56 */
/* name_ent_save_flag_clear - NAME_ENT overlay, Address: 0x8013CA90, Size: 48 bytes (12 insns) */
#include "types.h"

extern void save_flag_clear();
extern void goto_sub_sub_step();
extern s32 pad_trig;

void name_ent_save_flag_clear(void)
{
    if ((pad_trig & 0x0020) != 0) {
        save_flag_clear();
        goto_sub_sub_step(0);
    }
}
