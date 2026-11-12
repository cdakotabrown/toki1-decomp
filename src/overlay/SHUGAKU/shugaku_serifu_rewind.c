/* @SymbolStart: 0x80139644 */
/* @SymbolSize: 92 */
/* Address: 0x80139654, Size: 84 bytes (21 instructions). */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8  shugaku_place;
extern u8  date_place;
extern s16 now_serifu_shugaku;
extern u8  now_sub_sub_step;

void shugaku_serifu_rewind(void)
{
    s32 v0_param;
    s32 t7_param;

    v0_param = shugaku_place;
    t7_param = date_place;
    now_serifu_shugaku = (s16)(v0_param + 26);
    if (t7_param != 0) {
        now_serifu_shugaku = (s16)(v0_param + 3);
    }
    now_sub_sub_step = now_sub_sub_step - 46;
    sub_sub_step_up();
}
