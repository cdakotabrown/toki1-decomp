/* Address: 0x80153CD8 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[39])(s32);
} FnTable;

extern FnTable date_girl7_mode3_dispatch_tbl;
extern u8 now_sub_sub_step;
extern u8 fade_bri;

void date_girl7_mode3_dispatch(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl7_mode3_dispatch_tbl;
    if (((u32)now_sub_sub_step >= 27u || (u32)now_sub_sub_step < 8u) &&
        fade_bri == 128) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](128);
}
