/* @SymbolStart: 0x8013A4A4 */
/* @SymbolSize: 164 */
#include "types.h"

extern u8 shugaku_place;

extern void dec_bg_cache_step(s32 a0);
extern void strcpy(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

extern u8 ba_string;
extern u8 shugaku_place_bg_set4_str0;
extern u8 shugaku_place_bg_set4_str1;
extern u8 shugaku_place_bg_set4_str2;

void shugaku_place_bg_set4(void)
{
    switch (shugaku_place) {
    case 0:
        dec_bg_cache_step(16999);
        strcpy((s32)&ba_string, (s32)&shugaku_place_bg_set4_str0);
        break;
    case 1:
        dec_bg_cache_step(16981);
        strcpy((s32)&ba_string, (s32)&shugaku_place_bg_set4_str1);
        break;
    default:
        dec_bg_cache_step(17019);
        strcpy((s32)&ba_string, (s32)&shugaku_place_bg_set4_str2);
        break;
    }
    sub_sub_step_up();
}
