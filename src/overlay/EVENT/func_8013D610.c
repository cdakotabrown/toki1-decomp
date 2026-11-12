/* Address: 0x8013D610 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[39];
} DT39;

extern DT39 D_801222AC;
extern u8 D_800B1AF6;

void func_8013D610(void)
{
    u8 idx;
    DT39 table;

    table = D_801222AC;
    idx = D_800B1AF6;
    table.ptrs[idx](128);
}
