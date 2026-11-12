/* Address: 0x8015A278 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[52])(s32);
} FnTable;

extern FnTable date_girl10_main6_tbl;
extern u8 now_sub_sub_step;

void date_girl10_mode6_dispatch(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl10_main6_tbl;
    if ((u32)now_sub_sub_step >= 29u && (u32)now_sub_sub_step < 38u) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](60);
}
