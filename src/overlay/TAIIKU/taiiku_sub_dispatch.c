/* Address: 0x80133C80 */
/* Size: 124 bytes (31 insns) */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: swap_registers v0:t9 */
/* @PostProc: swap_registers v1:v0 */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
#include "types.h"

typedef struct {
    s32 entries[10];
} Table10;

extern Table10 taiiku_kari_step_tbl;
extern u8 now_sub_step;

void taiiku_sub_dispatch(void) {
    Table10 table;
    s32 (*fn)(void);

    table = taiiku_kari_step_tbl;
    fn = (s32 (*)(void))table.entries[now_sub_step];
    fn();
}
