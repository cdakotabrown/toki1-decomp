/* Address: 0x80138648 */
/* @SymbolSize: 164 */
/* @PostProc: patch_stack_complete --old 128 --new 144 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
#include "types.h"

extern void fade_in_step(void);
extern void device_cmd_send(s32);
extern void set_XAsector(s32, s32, s32);

typedef struct {
    s32 x;
    s32 y;
} Entry;

typedef struct {
    Entry e[12];
} Table;

extern Table date_basyo15_xa_start_tbl;
extern u8 step_mode;
extern u8 date_zoo_season;

void date_basyo15_xa_start(void) {
    Table local;
    u32 v1;
    s32 cond;

    local = date_basyo15_xa_start_tbl;
    fade_in_step();
    v1 = step_mode;
    cond = (u32)v1 < 1u;
    step_mode = v1 + 1;
    if (cond != 0) {
        device_cmd_send(191);
        set_XAsector(local.e[date_zoo_season].x, local.e[date_zoo_season].y, 15);
        device_cmd_send(768);
    }
}
