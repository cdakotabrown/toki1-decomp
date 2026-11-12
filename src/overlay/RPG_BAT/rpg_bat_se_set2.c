/* Address: 0x8013F0F4, Size: 104 bytes (26 insns) */
/* @SymbolSize: 104 */
/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 44 */
#include "types.h"

extern volatile s32 rpg_bat_se_wait[];

extern void device_cmd_send(u16);

void rpg_bat_se_set2(s32 a0, s32 a1, s32 a2)
{
    volatile s32 *ptr;
    s32 v;

    if (a1 == 0) {
        a1 = -1;
    }
    ptr = &rpg_bat_se_wait[a2];
    v = *ptr;
    if (v == 0) {
        device_cmd_send((u16)((a0 & 0xFF) | 0x600));
        *ptr = a1;
        v = a1;
    }
    *ptr = v - 1;
}
