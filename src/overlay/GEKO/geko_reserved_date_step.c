/* @SymbolStart: 0x801366C8 */
/* @SymbolSize: 160 */
/* geko_reserved_date_step -- GEKO overlay, Address: 0x801366D0, Size: 152 bytes (38 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 reserved_date_check(s32 a0, s32 a1, s32 a2);

extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern s32 reserved_date;
extern u8 now_girl;
extern u8 now_sub_sub_step;
extern s16 geko_now_serifu2;
extern u8 geko_girl_flag;

s32 geko_reserved_date_step(void)
{
    s32 t6 = geko_girl_flag;

    s32 v0;

    if (t6 == 0) {
        now_sub_sub_step = (u8)(now_sub_sub_step + 5);
    } else {
        geko_now_serifu2 = 19;
        v0 = reserved_date_check(now_year, now_month, now_day);
        if (v0 > 0) {
            if (((u32)(reserved_date << 3) >> 28) == now_girl) {
                goto_sub_sub_step(61);
                return 0;
            }
        }
    }
    return sub_sub_step_up();
}
