/* Address: 0x80156428 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[56])(s32);
} FnTable;

extern FnTable date_girl5_main6_tbl;
extern u8 now_sub_sub_step;

void date_girl5_main6(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl5_main6_tbl;
    if ((u32)now_sub_sub_step >= 30u && (u32)now_sub_sub_step < 44u) {
        date_light_effect();
    }
    local.f[now_sub_sub_step](128);
}
