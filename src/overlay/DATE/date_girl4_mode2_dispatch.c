/* Address: 0x8015906C */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[38])(s32);
} FnTable;

extern FnTable date_girl4_main2_tbl;
extern u8 now_sub_sub_step;
extern u8 fade_bri;

void date_girl4_mode2_dispatch(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl4_main2_tbl;
    if ((u32)now_sub_sub_step < 8u && fade_bri == 128) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](128);
}
