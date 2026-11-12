/* Address: 0x8013CCEC */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[40];
} JumpTable;

extern u8 D_800B1AF6;
extern void *D_8011F7C0;
extern JumpTable D_80121EE8;

extern void func_80078950();

void func_8013CCEC(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80121EE8;

    func_80078950(&D_8011F7C0, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);
}
