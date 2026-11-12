/* Address: 0x8013CF04 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[51];
} JumpTable;

extern JumpTable D_80122020;
extern s32 D_8011F7CC;
extern u8 D_800B1AF6;

extern void func_80078950(s32 a0, u8 a1);

void func_8013CF04(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80122020;

    func_80078950((s32)&D_8011F7CC, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);
}
