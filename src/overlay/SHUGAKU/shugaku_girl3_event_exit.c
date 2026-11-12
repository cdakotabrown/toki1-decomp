/* @SymbolStart: 0x80134060 */
/* @SymbolSize: 88 */
#include "types.h"

extern void param_clamp(void);
extern void device_cmd_send(s32);
extern void shugaku_ret_goto_or_exit(void);

extern u8 now_girl;
extern u8 screen_w[];

void shugaku_girl3_event_exit(void)
{
    s16 *p;

    p = (s16 *)&screen_w[now_girl * 56 + 450];
    *p = *p + 2;
    param_clamp();
    device_cmd_send(513);
    shugaku_ret_goto_or_exit();
}
