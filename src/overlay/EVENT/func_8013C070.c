/* Address: 0x8013C070 */
/* Size: 156 bytes (39 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[38];
} JumpTable;

extern u8 D_800B1AF6;
extern u8 D_800E9E63;
extern s32 D_8011F77C;
extern JumpTable D_80121DFC;

extern void func_80078950(s32 a0, u8 a1, u8 a2);

void func_8013C070(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80121DFC;

    func_80078950((s32)&D_8011F77C, D_800B1AF6, D_800E9E63);

    table.ptrs[D_800B1AF6](128);
}
