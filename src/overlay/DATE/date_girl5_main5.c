/* Address: 0x8015633C */
#include "types.h"

extern void date_spring(void);
extern void date_spring_clear(void);

typedef struct {
    void (*f[40])(s32);
} FnTable;

extern FnTable date_girl5_main5_tbl;
extern u8 now_sub_sub_step;

void date_girl5_main5(void) {
    s32 pad_hi;
    FnTable local;

    local = date_girl5_main5_tbl;
    if ((u32)now_sub_sub_step < 11u) {
        date_spring();
    }
    if (now_sub_sub_step == 11) {
        date_spring_clear();
    }
    local.f[now_sub_sub_step](128);
}
