/* Address: 0x80153FF0 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[42])(s32);
} FnTable;

extern FnTable date_girl7_mode5_dispatch_tbl;
extern u8 now_sub_sub_step;

void date_girl7_mode5_dispatch(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl7_mode5_dispatch_tbl;
    if ((u32)now_sub_sub_step < 9u) {
        date_light_effect();
    }
    if ((u32)now_sub_sub_step >= 22u && (u32)now_sub_sub_step < 28u) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](128);
}
