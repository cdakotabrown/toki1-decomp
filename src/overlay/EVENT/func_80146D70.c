/* Address: 0x80146D70 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[14];
} JumpTable;

extern u8 D_800B1AF6;
extern char D_8011FB44;
extern JumpTable D_80123F94;

extern void func_80078950(char *s, u8 a1);

void func_80146D70(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80123F94;

    func_80078950(&D_8011FB44, D_800B1AF6);
    table.ptrs[D_800B1AF6](128);
}
