/* @SymbolSize: 68 */
/* name_ent_bgm_load - NAME_ENT overlay, Address: 0x80144ED4, Size: 68 bytes (17 insns) */
#include "types.h"

extern void device_load_start();
extern void sub_sub_step_up();

void name_ent_bgm_load(void)
{
    device_load_start(0, 0xC5A0, 0xC58E, 0xD16D, 0xD11E, 0xD110);
    sub_sub_step_up();
}
