/* @CompileFlags: -cckr */
/* @SymbolSize: 152 */
/* shugaku_birth_zyotai_serifu - SHUGAKU overlay, Size: 152 bytes. */
#include "types.h"

extern u8  now_girl;
extern s16 shugaku_birth_now_serifu;

extern s32  get_g_zyotai_s(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_birth_zyotai_serifu(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s(now_girl);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 >= 2U) {
        if (v1 == 2) {
            shugaku_birth_now_serifu += 2;
        } else if (v1 == 3) {
            shugaku_birth_now_serifu += 4;
        } else {
            shugaku_birth_now_serifu += 6;
        }
    }
    sub_sub_step_up();
}
