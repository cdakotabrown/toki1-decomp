/* func_80146E7C -- EVENT overlay, Address: 0x80146E7C, Size: 196 bytes (49 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[56];
} JumpTable;

extern void func_8003C2C0(void);
extern void func_8010AFE4(void);

extern u8 D_800B1AF6;
extern JumpTable D_80123FCC;

void func_80146E7C(void)
{
    s32 pad_hi;
    JumpTable table;
    u32 v;

    table = D_80123FCC;

    table.ptrs[D_800B1AF6](128);

    v = D_800B1AF6;
    if (v >= 30U && v < 37U) {
        func_8003C2C0();
        return;
    }
    if (v == 43U) {
        func_8010AFE4();
    }
}
