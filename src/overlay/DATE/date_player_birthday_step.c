/* Address: 0x80145C84 */
/* @PostProc: patch_stack_complete --old 88 --new 104 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[15])(s32);
} FnTable;

extern FnTable date_main3_tbl;
extern u8  now_sub_sub_step;
extern s32 date_disp_flag;
extern u8  fade_bri;

void date_player_birthday_step(void) {
    FnTable local;

    local = date_main3_tbl;
    local.f[now_sub_sub_step](128);

    if ((date_disp_flag & 1) && fade_bri == 128) {
        date_light_effect();
    }
}
