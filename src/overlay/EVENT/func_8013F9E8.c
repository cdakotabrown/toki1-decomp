/* Address: 0x8013F9E8 */
/* Size: 156 bytes (39 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[40];
} JumpTable;

extern u8 D_800B1AF6;
extern u16 D_80094714;
extern u16 D_80094718;
extern s32 D_8011F8C4;
extern JumpTable D_801229B0;

extern void func_80078950(s32 a0, u8 a1, u16 a2, u16 a3);

void func_8013F9E8(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_801229B0;

    func_80078950((s32)&D_8011F8C4, D_800B1AF6, D_80094714, D_80094718);

    table.ptrs[D_800B1AF6](128);
}
