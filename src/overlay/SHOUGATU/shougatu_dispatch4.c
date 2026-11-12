/* shougatu_dispatch4 - SHOUGATU overlay, Address: 0x801412F4, Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[14];
} DispatchTable14;

extern DispatchTable14 shougatu_dispatch4_tbl;
extern u8 now_sub_sub_step;

void shougatu_dispatch4(void)
{
    u8 idx;
    DispatchTable14 table;

    table = shougatu_dispatch4_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
