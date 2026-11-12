/* Address: 0x8015919C */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[56])(s32);
} FnTable;

extern FnTable date_girl4_main3_tbl;
extern s32 date_sprite_on;
extern u8  fade_bri;
extern u8  now_sub_sub_step;

void date_girl4_mode3_dispatch(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl4_main3_tbl;
    if (date_sprite_on == 0 && fade_bri == 128) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](128);
}
