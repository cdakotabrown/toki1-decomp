/* @SymbolStart: 0x80133470 */
/* @SymbolSize: 132 */
/* ending_girl_route0d - ENDING overlay, Address: 0x80133470, Size: 132 bytes (33 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 girl14_flag;
extern u8 sys_flags;
extern void goto_sub_step(s32);
extern void sub_sub_step_up(void);

void ending_girl_route0d(void)
{
    u8 step = now_girl;
    s32 v;
    u8 flags;

    if ((u32)step >= 13U) {
        v = girl14_flag;
        if ((u32)((u32)(v << 20) >> 29) < 3U) {
            goto_sub_step(7);
            flags = sys_flags;
            sys_flags = flags | 0x08;
            return;
        }
    }
    sub_sub_step_up();
    flags = sys_flags;
    sys_flags = flags | 0x04;
}
