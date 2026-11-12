/* Address: 0x8013E950 */
/* Size: 156 bytes (39 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[32];
} JumpTable;

extern u8 D_800B1AF6;
extern u8 D_800E9E63;
extern s32 D_8011F898;
extern JumpTable D_8012277C;

extern void func_80078950(s32 a0, u8 a1, u8 a2);

void func_8013E950(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_8012277C;

    func_80078950((s32)&D_8011F898, D_800B1AF6, D_800E9E63);

    table.ptrs[D_800B1AF6](128);
}
