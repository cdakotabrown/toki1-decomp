/* @SymbolSize: 136 */
/* shougatu_dispatch2 - SHOUGATU overlay, Address: 0x801410FC, Size: 120 bytes (30 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[14];
} DispatchTable14;

extern DispatchTable14 shougatu_dispatch2_tbl;
extern u8 now_sub_sub_step;

void shougatu_dispatch2(void)
{
    u8 idx;
    DispatchTable14 table;

    table = shougatu_dispatch2_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
