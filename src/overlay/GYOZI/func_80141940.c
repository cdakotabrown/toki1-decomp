/* Address: 0x80141940 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[11];
} DispatchTable11;

extern DispatchTable11 D_80148B04;
extern u8 D_800F647A;

void func_80141940(void)
{
    u8 idx;
    DispatchTable11 table;

    table = D_80148B04;
    idx = D_800F647A;
    table.ptrs[idx](128);
}
