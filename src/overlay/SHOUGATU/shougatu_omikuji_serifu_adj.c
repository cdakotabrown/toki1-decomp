/* @SymbolStart: 0x80135000 */
/* @SymbolSize: 104 */
/* Address: 0x80135008 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_omikuji_no;

void shougatu_omikuji_serifu_adj(void)
{
    if (shougatu_omikuji_no == 0) {
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 2;
    } else if (shougatu_omikuji_no != 3 && shougatu_omikuji_no != 4) {
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 1;
    }
    sub_sub_step_up();
}
