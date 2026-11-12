/* Address: 0x8013BC30 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[39];
} DT39_BC30;

extern DT39_BC30 geko_sub_sub_dispatch39b_tbl;
extern u8 now_sub_sub_step;

void geko_sub_sub_dispatch39b(void)
{
    u8 idx;
    DT39_BC30 table;

    table = geko_sub_sub_dispatch39b_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
