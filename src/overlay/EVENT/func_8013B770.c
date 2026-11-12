/* Address: 0x8013B770 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable;

extern JumpTable D_80121BB8;
extern s32 D_8011F748;
extern u8 D_800B1AF6;

extern void func_80078950(s32 a0, u8 a1);

void func_8013B770(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80121BB8;

    func_80078950((s32)&D_8011F748, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);
}
