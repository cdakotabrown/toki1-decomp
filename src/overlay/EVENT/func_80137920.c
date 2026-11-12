/* Address: 0x80137920 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[45];
} DT45;

extern DT45 D_8012112C;
extern u8 D_800B1AF6;

void func_80137920(void)
{
    u8 idx;
    DT45 table;

    table = D_8012112C;
    idx = D_800B1AF6;
    table.ptrs[idx](128);
}
