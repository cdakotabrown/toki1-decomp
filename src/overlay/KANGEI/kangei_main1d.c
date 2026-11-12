/* @PostProc: patch_stack_complete --old 128 --new 144 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* @SymbolSize: 156 */
/* Address: 0x80136D60 */
/* Size: 156 bytes (39 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[24];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable kangei_main1d_tbl;

extern void kangei_scroll(void);

void kangei_main1d(void)
{
    JumpTable table;
    u32 idx;

    table = kangei_main1d_tbl;

    table.ptrs[now_sub_sub_step](128);

    idx = now_sub_sub_step;
    if (idx < 9) {
        return;
    }
    if (idx >= 20) {
        return;
    }
    kangei_scroll();
}
