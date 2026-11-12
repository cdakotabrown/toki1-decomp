/* Address: 0x80157164 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[61])(s32);
} FnTable;

extern FnTable date_girl6_main3_tbl;
extern u8 now_sub_sub_step;
extern u8 fade_bri;

void date_girl6_mode3_dispatch(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl6_main3_tbl;
    if (((u32)now_sub_sub_step < 4u || (u32)now_sub_sub_step >= 44u) &&
        fade_bri == 128) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](128);
}
