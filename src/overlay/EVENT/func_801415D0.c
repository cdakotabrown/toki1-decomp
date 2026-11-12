/* Address: 0x801415D0 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[45];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_8012304C;

extern void func_80105DF8(void);

void func_801415D0(void)
{
    u8 idx;
    JumpTable table;

    table = D_8012304C;

    idx = D_800B1AF6;
    table.ptrs[idx](128);

    func_80105DF8();
}
