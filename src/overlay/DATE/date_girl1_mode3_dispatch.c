/* Address: 0x80154E78 */
#include "types.h"

extern void date_light_disp(void);
extern void date_light_effect(void);

typedef struct {
    void (*f[42])(s32);
} FnTable;

extern FnTable date_girl1_main3_tbl;
extern u8  now_sub_sub_step;
extern s32 date_sprite_on;
extern u8  fade_bri;

void date_girl1_mode3_dispatch(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl1_main3_tbl;
    if ((u32)now_sub_sub_step >= 15u && (u32)now_sub_sub_step < 18u) {
        date_light_disp();
    }
    if (date_sprite_on == 0 && fade_bri == 128) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](128);
}
