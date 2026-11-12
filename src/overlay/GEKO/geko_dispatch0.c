/* Address: 0x801323E0 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[33];
} DT33_323E0;

extern DT33_323E0 geko_dispatch0_tbl;
extern u8 now_sub_sub_step;

void geko_dispatch0(void)
{
    u8 idx;
    DT33_323E0 table;

    table = geko_dispatch0_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
