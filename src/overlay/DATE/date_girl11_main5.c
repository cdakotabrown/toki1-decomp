/* Address: 0x8015A57C */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[20])(s32);
} FnTable;

extern FnTable date_girl11_main5_tbl;
extern u8 now_sub_sub_step;

void date_girl11_main5(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl11_main5_tbl;
    date_light_effect();
    local.f[now_sub_sub_step](128);
}
