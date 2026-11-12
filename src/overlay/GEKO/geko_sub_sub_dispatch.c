/* Address: 0x8013D11C */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[39];
} DT39_D11C;

extern DT39_D11C geko_sub_sub_dispatch_tbl;
extern u8 now_sub_sub_step;

void geko_sub_sub_dispatch(void)
{
    u8 idx;
    DT39_D11C table;

    table = geko_sub_sub_dispatch_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
