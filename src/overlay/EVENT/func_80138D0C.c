/* Address: 0x80138D0C */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable;

extern u8 D_800B1AF6;
extern void *D_8011F6A4;
extern JumpTable D_80121360;

extern void func_80078950();

void func_80138D0C(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80121360;

    func_80078950(&D_8011F6A4, D_800B1AF6);

    table.ptrs[D_800B1AF6]();
}
