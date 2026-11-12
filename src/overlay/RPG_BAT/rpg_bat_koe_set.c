/* @SymbolStart: 0x8013F1C4 */
/* @SymbolSize: 92 */
#include "types.h"

extern u8 now_girl;
extern s32 rpg_bat_talk_work[];
extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void device_cmd_send(s32 a0);

void rpg_bat_koe_set(s32 arg0, s32 arg1, s32 idx)
{
    s32 *p = &rpg_bat_talk_work[idx];

    if (*p == 0) {
        now_girl = 14;
        set_XAsector(arg0, arg1, 14);
        device_cmd_send(768);
        *p = 1;
    }
}
