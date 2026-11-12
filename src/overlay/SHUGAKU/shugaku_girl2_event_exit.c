/* @SymbolStart: 0x80135910 */
/* @SymbolSize: 52 */
/* shugaku_girl2_event_exit - SHUGAKU overlay, Address: 0x80135910, Size: 52 bytes (13 insns) */
#include "types.h"

extern void goto_sub_step(s32);
extern void device_cmd_send(s32);
extern u8 shugaku_rpg_arg;

void shugaku_girl2_event_exit(void)
{
    u8 flag = shugaku_rpg_arg;

    if (flag == 0) {
        device_cmd_send(0x202);
    }
    goto_sub_step(5);
}
