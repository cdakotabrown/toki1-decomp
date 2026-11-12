/* @SymbolStart: 0x80145D60 */
/* @SymbolSize: 132 */
/* @CompileFlags: -Wo,-notail */
#include "types.h"

extern s32 get_g_zyotai_s(u8);
extern void goto_sub_sub_step(s32);
extern void event_log_add(s32, u8);
extern void sub_sub_step_up(void);

extern u8 day_flag[];
extern u8 now_date_spot;

void date_main3_zyotai_check(void)
{
    u8 result;

    day_flag[0] = day_flag[0] | 8;
    result = get_g_zyotai_s(day_flag[0x841]);

    if ((u32)(result & 0x7F) >= 2) {
        goto_sub_sub_step(9);
    } else {
        day_flag[0] = day_flag[0] | 4;
        event_log_add(13, now_date_spot);
        sub_sub_step_up();
    }
}
