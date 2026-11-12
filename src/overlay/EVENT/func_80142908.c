/* Address: 0x80142908 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[63];
} JumpTable;

extern JumpTable D_8012320C;
extern s32 D_8011F988;
extern u8 D_800B1AF6;

extern void func_80078950(s32 a0, u8 a1);

void func_80142908(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_8012320C;

    func_80078950((s32)&D_8011F988, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);
}
