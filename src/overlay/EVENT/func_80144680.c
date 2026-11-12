/* Address: 0x80144680 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[21];
} DT21_44680;

extern DT21_44680 D_80123A4C;
extern u8 D_800B1AF6;

void func_80144680(void)
{
    u8 idx;
    DT21_44680 table;

    table = D_80123A4C;
    idx = D_800B1AF6;
    table.ptrs[idx](128);
}
