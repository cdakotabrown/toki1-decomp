/* @PostProc: patch_stack_complete --old 128 --new 136 */
/* @PostProc: remap_stack_slots --from 36 --to 40 */
/* @SymbolSize: 176 */
/* Address: 0x8013A130, Size: 176 bytes (44 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[23];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch23_tbl;
extern void date_light_effect(void);

void geko_sub_sub_dispatch23(void)
{
    JumpTable table;
    s32 idx;

    table = geko_sub_sub_dispatch23_tbl;

    table.ptrs[now_sub_sub_step]();

    idx = now_sub_sub_step;
    if ((u32)idx < 13U) {
    } else if ((u32)idx < 20U) {
        date_light_effect();
    }
}
