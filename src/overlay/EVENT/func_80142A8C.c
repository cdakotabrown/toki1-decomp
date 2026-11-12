/* Address: 0x80142A8C */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[36];
} JumpTable;

extern JumpTable D_80123308;
extern s32 D_8011F98C;
extern u8 D_800B1AF6;

extern void func_80078950(s32 a0, u8 a1);

void func_80142A8C(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80123308;

    func_80078950((s32)&D_8011F98C, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);
}
