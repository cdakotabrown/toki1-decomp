/* geko_sub_dispatch28 - GEKO overlay, Address: 0x80140DB0, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[28];
} DispatchTable25;

extern DispatchTable25 geko_sub_dispatch28_tbl;
extern u8 now_sub_sub_step;

void geko_sub_dispatch28(void)
{
    u8 idx;
    DispatchTable25 table;

    table = geko_sub_dispatch28_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
