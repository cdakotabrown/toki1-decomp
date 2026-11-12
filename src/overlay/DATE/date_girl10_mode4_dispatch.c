/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=9:a0,14:v1 */
/* Address: 0x80159F9C, Size: 360 bytes (90 insns) */
#include "types.h"

extern void date_light_effect(void);
extern void date_cmd1280_send2(void);

typedef struct {
    void (*f[46])(s32);
} FnTable;

extern FnTable date_girl10_main4_tbl;
extern volatile u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 fade_bri;
extern s32 date_sprite_on;
extern u8 now_sub_sub_step;
extern s32 frame_counter;

void date_girl10_mode4_dispatch(void)
{
    s32 pad;
    FnTable local;
    s32 pad2[2];
    u32 idx;
    s32 a;

    a = now_year;
    local = date_girl10_main4_tbl;

    if (((a == 95 && now_month == 7 && now_day == 23) ||
         (a == 96 && now_month == 7 && now_day == 28) ||
         (a == 97 && now_month == 7 && now_day == 27)) &&
        fade_bri == 128 && date_sprite_on == 0) {
        idx = now_sub_sub_step;
        if (idx < 13U || idx >= 33U) {
            date_light_effect();
        }
    }

    idx = now_sub_sub_step;
    local.f[idx](128);

    if (date_sprite_on != 0 && (u32)frame_counter % 150U == 0) {
        date_cmd1280_send2();
    }
}
