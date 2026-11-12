/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 316 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=15:a0,20:v1 */
/* Function: date_girl7_mode6_dispatch */
/* Overlay: DATE */
#include "types.h"

typedef s32 (*DateFn)(s32);

struct DateFnTable {
    DateFn fns[15];
};

extern struct DateFnTable date_girl7_mode6_dispatch_tbl;
extern u8 now_sub_sub_step;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 fade_bri;
extern u8 now_basyo;
extern void date_light_effect(void);

s32 date_girl7_mode6_dispatch(void)
{
    s32 pad;
    struct DateFnTable buf;
    u32 idx;
    s32 a;
    s32 d;

    buf = date_girl7_mode6_dispatch_tbl;
    idx = now_sub_sub_step;
    if (idx < 12) {
        a = now_year;
        if ((a == 95 && now_month == 7 && now_day == 23) ||
            (a == 96 && now_month == 7 && now_day == 28) ||
            (a == 97 && now_month == 7 && now_day == 27)) {
            if (fade_bri == 128) {
                date_light_effect();
                goto done;
            }
        }
        d = now_basyo;
        if (d == 17 || d == 16) {
            date_light_effect();
        }
    }
done:
    idx = now_sub_sub_step;
    return buf.fns[idx](128);
}
