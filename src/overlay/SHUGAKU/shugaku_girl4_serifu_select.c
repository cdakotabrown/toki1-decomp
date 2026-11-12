/* @SymbolStart: 0x80138BE8 */
/* @SymbolSize: 112 */
#include "types.h"

extern void func_80138664(void);

extern u8  shugaku_zyotai4_girl;
extern u16 basyo_pack;
extern u8  girl4_basyo;
extern s16 now_serifu_shugaku;
extern u8  step_mode;

void shugaku_girl4_serifu_select(void)
{
    u16 tv;
    u8  tv2;

    if (step_mode != 0) goto call;
    if (shugaku_zyotai4_girl != 4) goto set;
    tv2 = girl4_basyo;
    tv = basyo_pack;
    if (((u32)tv2 >> 4) != ((u32)tv >> 12)) goto set;
    now_serifu_shugaku = 46;
set:
    step_mode = 1;
call:
    func_80138664();
}
