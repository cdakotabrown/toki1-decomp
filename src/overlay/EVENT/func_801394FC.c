/* Address: 0x801394FC */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[51];
} JumpTable;

extern u8 D_800B1AF6;
extern u8 D_800E9E63;
extern void *D_8011F6F4;
extern JumpTable D_801215FC;

extern void func_80078950();

void func_801394FC(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_801215FC;

    func_80078950(&D_8011F6F4, D_800B1AF6, D_800E9E63);

    table.ptrs[D_800B1AF6](128);
}
