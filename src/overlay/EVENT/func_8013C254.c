/* Address: 0x8013C254 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[21];
} DT21;

extern DT21 D_80121E94;
extern u8 D_800B1AF6;

void func_8013C254(void)
{
    u8 idx;
    DT21 table;

    table = D_80121E94;
    idx = D_800B1AF6;
    table.ptrs[idx](128);
}
