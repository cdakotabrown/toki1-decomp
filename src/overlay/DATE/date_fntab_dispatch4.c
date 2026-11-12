/* Address: 0x8014FB90 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[43])(void);
} FnTable;

extern FnTable date_fntab_dispatch4_tbl;
extern u8 now_sub_sub_step;
extern u8 fade_bri;

void date_fntab_dispatch4(void) {
    s32 pad_hi;
    FnTable local;

    local = date_fntab_dispatch4_tbl;
    if ((u32)now_sub_sub_step < 10u && fade_bri == 128) {
        date_light_effect();
    }
    local.f[now_sub_sub_step]();
}
