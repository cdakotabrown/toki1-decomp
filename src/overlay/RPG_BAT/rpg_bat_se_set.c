/* @SymbolSize: 104 */
/* rpg_bat_se_set - RPG_BAT overlay, Address: 0x8013F15C, Size: 104 bytes (26 insns) */
/* @PostProc: remap_stack_slots --from 40 --to 44 */
#include "types.h"

extern s32 rpg_bat_se_wait[];
extern s32 device_cmd_send();

void rpg_bat_se_set(s32 a0, s32 a1, s32 a2)
{
    s32 *ptr;

    if (a1 == 0) {
        a1 = -1;
    }
    ptr = &rpg_bat_se_wait[a2];
    if (*ptr == 0) {
        device_cmd_send((u16)(((u32)a0 & 0xFF) | 0x500));
        *ptr = a1;
    }
    *ptr = *ptr - 1;
}
