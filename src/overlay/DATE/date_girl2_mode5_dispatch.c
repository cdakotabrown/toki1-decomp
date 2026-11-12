/* Address: 0x80151354 */
/* @PostProc: patch_stack_complete --old 184 --new 208 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    void (*f[40])(s32);
} FnTable;

extern FnTable date_girl2_mode5_dispatch_tbl;
extern u8  now_sub_sub_step;
extern s32 date_sprite_on;
extern u8  fade_bri;
extern CLASS_W class_w[];

void date_girl2_mode5_dispatch(void) {
    FnTable local;

    local = date_girl2_mode5_dispatch_tbl;
    local.f[now_sub_sub_step](128);
    if (date_sprite_on != 0) {
        class_w[98].disp_flag = class_w[98].disp_flag | 0x80;
        class_w[98].bright = fade_bri;
    } else {
        class_w[98].disp_flag = class_w[98].disp_flag & 0xFF7F;
    }
}
