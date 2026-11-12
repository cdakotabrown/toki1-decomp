/* Address: 0x80153D80 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern s32 get_g_zyotai_h(s32 a0);
extern void func_8014C530(void);

extern u16 now_serifuev;

void date_serifuev_zyotai_set(void)
{
    s32 v0;

    v0 = get_g_zyotai_h(7);
    now_serifuev = (u16)(((u32)v0 >= 1) + ((u32)v0 >= 2) + ((u32)v0 >= 3) + 1);
    func_8014C530();
}
