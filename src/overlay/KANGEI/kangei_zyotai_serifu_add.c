/* @CompileFlags: -cckr */
/* @SymbolSize: 144 */
/* kangei_zyotai_serifu_add - KANGEI overlay, Address: 0x80138728, Size: 144 bytes (36 insns) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);
extern s16 kangei_now_s;

void kangei_zyotai_serifu_add(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s(0);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if (v1 == 2) {
        kangei_now_s += 2;
    } else if (v1 == 3) {
        kangei_now_s += 4;
    } else if (v1 == 4) {
        kangei_now_s += 6;
    }
    sub_sub_step_up();
}
