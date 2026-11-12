/* @CompileFlags: -cckr */
/* @SymbolStart: 0x801395B0 */
/* @SymbolSize: 148 */
/* shugaku_zyotai_serifu_add - SHUGAKU overlay, Size: 148 bytes (head at 0x801395B0). */
#include "types.h"

extern u8 date_place;
extern u8  now_girl;
extern s16 now_serifu_shugaku;

extern s32  get_g_zyotai_s(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_zyotai_serifu_add(void)
{
    s32 v0;
    s32 v1;
    u8 masked;
    s32 n;

    if (date_place == 0) {
        v0 = get_g_zyotai_s(now_girl);
        v1 = v0;
        masked = (u8)(v1 & 0x7F);
        v1 = masked;
        if ((u32)v1 < 2U) {
            n = 0;
        } else if (v1 == 2) {
            n = 3;
        } else if (v1 == 3) {
            n = 6;
        } else {
            n = 9;
        }
        now_serifu_shugaku += n;
    }
    sub_sub_step_up();
}
