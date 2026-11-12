/* @SymbolStart: 0x80133778 */
/* date2_scroll_set -- DATE2 overlay, Address: 0x80133780, Size: 248 bytes (62 insns) */
/* @SymbolSize: 256 */
#include "types.h"

extern u8 date2_mode_no;

extern u8 date2_mode_no;

extern s16 date2_g0_now_s;
extern s16 date2_girl_now_s;
extern s16 date2_now_s;
extern u8  now_year;

void date2_scroll_add(void);
void sub_sub_step_up(void);

void date2_scroll_set(void)
{
    u8 v0;
    s16 t1;

    v0 = date2_mode_no;

    switch (v0)
    {
    case 0:
        date2_g0_now_s = 1;
        date2_girl_now_s = 1;
        break;
    case 1:
        if ((u32)now_year >= 97U)
        {
            date2_g0_now_s = 5;
            t1 = date2_now_s;
            date2_girl_now_s = 5;
            date2_now_s = (s16)(t1 + 2);
        }
        else
        {
            date2_g0_now_s = 3;
            date2_girl_now_s = 3;
        }
        break;
    case 2:
        date2_g0_now_s = 7;
        date2_girl_now_s = 7;
        if ((u32)now_year < 97U)
        {
            t1 = date2_now_s;
            date2_now_s = (s16)(t1 + 2);
        }
        break;
    }
    date2_scroll_add();
    sub_sub_step_up();
}
