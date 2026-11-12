/* Address: 0x80135DF8 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[39];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80120D30;

extern void func_800FA25C(void);

void func_80135DF8(void)
{
    u8 idx;
    JumpTable table;

    table = D_80120D30;

    idx = D_800B1AF6;
    table.ptrs[idx](128);

    func_800FA25C();
}
