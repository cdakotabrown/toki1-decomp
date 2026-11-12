/* @SymbolStart: 0x80136B70 */
/* @SymbolSize: 56 */
#include "types.h"

extern void device_cmd_send();
extern s32 en_nichi_cmd_wait_cnt;

void en_nichi_cmd_send_once(s32 arg0)
{
    if (en_nichi_cmd_wait_cnt < 1) {
        en_nichi_cmd_wait_cnt = 1;
        device_cmd_send(arg0 & 0xFFFF);
    }
}
