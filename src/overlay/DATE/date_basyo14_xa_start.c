/* Address: 0x80138384 */
/* @SymbolSize: 168 */
/* @PostProc: patch_stack_complete --old 120 --new 136 */
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
    Entry e[11];
} Table;

extern Table date_basyo14_xa_start_tbl;
extern u8 step_mode;
extern u8 date_parade_season;

void date_basyo14_xa_start(void) {
    Table local;
    u32 v1;
    s32 cond;

    local = date_basyo14_xa_start_tbl;
    fade_in_step();
    v1 = step_mode;
    cond = (u32)v1 < 1u;
    step_mode = v1 + 1;
    if (cond != 0) {
        device_cmd_send(191);
        set_XAsector(local.e[date_parade_season].x, local.e[date_parade_season].y, 15);
        device_cmd_send(768);
    }
}
