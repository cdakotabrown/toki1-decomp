/* Function: date_fade_in */
/* Address: 0x80085570 */
/* Size: 156 bytes (39 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void fade_in_step(void);
extern void date_sprite_switch(s32 a0);
extern void mes_window_switch(s32 a0);
extern void sprite_rgb_set(s16 a0, u8 a1);

extern u8 fade_bri;
extern s32 date_sprite_on;
extern u8 cal_cw_bright;
extern u8 cw97_bright;

void date_fade_in(void) {
    s32 t6;
    s16 i;
    u8 val;
    s16 arg0;

    fade_in_step();

    val = fade_bri;
    t6 = 1;
    if (val == 4) {
        date_sprite_switch(1);
        t6 = 1;
    }

    date_sprite_on = t6;
    mes_window_switch(0);

    val = fade_bri;
    cal_cw_bright = val;
    cw97_bright = val;

    i = 58;
    do {
        arg0 = i;
        val = fade_bri;
        sprite_rgb_set(arg0, val);
        i++;
    } while (i < 64);
}
