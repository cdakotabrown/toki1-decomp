/* Address: 0x8014729C */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[14];
} JumpTable;

extern u8 D_800B1AF6;
extern char D_8011FB48;
extern JumpTable D_801240AC;

extern void func_80078950(char *s, u8 a1);

void func_8014729C(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_801240AC;

    func_80078950(&D_8011FB48, D_800B1AF6);
    table.ptrs[D_800B1AF6](128);
}
