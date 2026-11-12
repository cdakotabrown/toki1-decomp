/* @SymbolStart: 0x80139370 */
/* @SymbolSize: 60 */
/* shugaku_serifu_place_adjust - SHUGAKU overlay, Address: 0x80139380, Size: 44 bytes (11 insns) */
#include "types.h"
extern u8 shugaku_place;

extern s16 now_serifu_shugaku;
extern void sub_sub_step_up(void);

void shugaku_serifu_place_adjust(void)
{
    s32 t6;
    s32 t7;

    t6 = now_serifu_shugaku;
    t7 = shugaku_place;

    now_serifu_shugaku = (s16)((t6 - t7) + 2);
    sub_sub_step_up();

}
