/* Address: 0x801330C0 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[60];
} DT60_330C0;

extern DT60_330C0 shougatu_main53c_tbl;
extern u8 now_sub_sub_step;

void shougatu_main53c(void)
{
    u8 idx;
    DT60_330C0 table;

    table = shougatu_main53c_tbl;
    idx = now_sub_sub_step;
    table.ptrs[idx](128);
}
