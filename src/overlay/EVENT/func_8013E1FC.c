/* Address: 0x8013E1FC */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[46];
} JumpTable;

extern u8 D_800B1AF6;
extern void *D_8011F850;
extern JumpTable D_801224F0;

extern void func_80078950();

void func_8013E1FC(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_801224F0;

    func_80078950(&D_8011F850, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);
}
