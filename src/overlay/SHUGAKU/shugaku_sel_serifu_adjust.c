/* @SymbolStart: 0x8013A364 */
/* Address: 0x8013A36C, Size: 112 bytes (28 insns) */
/* @SymbolSize: 120 */
#include "types.h"

extern u8 date_place;

extern s16 now_serifu_shugaku;
extern s32 serifu_no;

extern void sub_sub_step_up(void);

void shugaku_sel_serifu_adjust(void)
{
    s32 flag = date_place;

    if (flag == 0) {
        now_serifu_shugaku = now_serifu_shugaku - serifu_no * 2 + 2;
    } else {
        now_serifu_shugaku = now_serifu_shugaku - serifu_no * 2 + 4;
    }
    sub_sub_step_up();
}
