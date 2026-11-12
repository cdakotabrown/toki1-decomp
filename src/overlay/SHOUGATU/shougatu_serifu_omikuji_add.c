/* @SymbolStart: 0x80134F98 */
/* @SymbolSize: 104 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_omikuji_no;

void shougatu_serifu_omikuji_add(void)
{
    if (shougatu_omikuji_no == 3 || shougatu_omikuji_no == 4) {
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 2;
    } else if (shougatu_omikuji_no != 0) {
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 1;
    }
    sub_sub_step_up();
}
