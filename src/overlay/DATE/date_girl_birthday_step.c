/* Address: 0x801494F4 */
/* Size: 172 bytes (43 insns) */
/* @SymbolSize: 172 */
/* @PostProc: patch_stack_complete --old 136 --new 160 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
#include "types.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[28])(s32);
} FnTable;

extern FnTable date_main2_tbl;
extern u8  now_sub_sub_step;
extern s32 date_disp_flag;
extern u8  fade_bri;

void date_girl_birthday_step(void) {
    FnTable local;

    local = date_main2_tbl;
    local.f[now_sub_sub_step](128);

    if ((date_disp_flag & 1) && fade_bri == 128) {
        date_light_effect();
    }
}
