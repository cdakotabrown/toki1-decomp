/* @PostProc: patch_stack_complete --old 152 --new 168 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* @SymbolSize: 116 */
/* Address: 0x801341F8 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[30];
} JumpTable801341F8;

extern JumpTable801341F8 kangei_switch_tbl;
extern u8 now_sub_sub_step;

void kangei_switch(void)
{
    JumpTable801341F8 table;
    s32 idx;

    table = kangei_switch_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](32);
}
