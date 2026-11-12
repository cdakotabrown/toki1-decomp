/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=9:a0,14:v1 */
/* date_girl9_mode4_dispatch -- DATE overlay, Address: 0x801582C8, Size: 268 bytes (67 insns) */
#include "types.h"

typedef s32 (*DateFn)(s32);

typedef struct {
    DateFn fns[63];
} DateFnTable;

extern volatile u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 fade_bri;
extern s32 date_sprite_on;
extern u8 now_sub_sub_step;
extern DateFnTable date_girl9_main4_tbl;

extern s32 date_light_effect();

s32 date_girl9_mode4_dispatch(void)
{
    s32 pad;
    DateFnTable buf;
    s32 pad2[2];
    s32 a;

    a = now_year;
    buf = date_girl9_main4_tbl;
    if ((a == 95 && now_month == 7 && now_day == 23) ||
        (a == 96 && now_month == 7 && now_day == 28) ||
        (a == 97 && now_month == 7 && now_day == 27)) {
        if (fade_bri == 128 && date_sprite_on == 0) {
            date_light_effect();
        }
    }
    return buf.fns[now_sub_sub_step](128);
}
