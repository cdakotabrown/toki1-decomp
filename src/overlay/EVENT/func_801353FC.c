/* Address: 0x801353FC */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80120A94;

void func_801353FC(void) {
    s32 pad_hi;
    JumpTable table;

    table = D_80120A94;
    table.ptrs[D_800B1AF6](128);
}
