/* @SymbolStart: 0x8013A32C */
/* @SymbolSize: 56 */
/* shugaku_serifu_place_add - SHUGAKU overlay, Address: 0x8013A33C, Size: 40 bytes (10 insns) */
#include "types.h"

extern s16 now_serifu_shugaku;
extern u8  shugaku_place;
extern void sub_sub_step_up(void);

void shugaku_serifu_place_add(void)
{
    now_serifu_shugaku = now_serifu_shugaku + shugaku_place;
    sub_sub_step_up();
}
