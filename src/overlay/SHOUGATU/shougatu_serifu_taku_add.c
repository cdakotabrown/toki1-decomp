/* @SymbolStart: 0x8013455C */
/* @SymbolSize: 116 */
/* Address: 0x80134564 */
/* Size: 108 bytes (27 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_taku_no;

void shougatu_serifu_taku_add(void)
{
    switch (shougatu_taku_no) {
    case 0:
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 3;
        break;
    case 1:
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 2;
        break;
    case 2:
    default:
        break;
    }
    sub_sub_step_up();
}
