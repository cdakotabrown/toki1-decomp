/* Address: 0x80146008 */
/* Size: 164 bytes (41 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[44];
} JumpTable;

extern u8 D_800B1AF6;
extern u16 D_80094714;
extern u16 D_80094718;
extern char D_8011FA90;
extern JumpTable D_80123D0C;

extern void func_80078950(char *s, u8 a1, u16 a2, u16 a3);

void func_80146008(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80123D0C;

    func_80078950(&D_8011FA90, D_800B1AF6, D_80094714, D_80094718);
    table.ptrs[D_800B1AF6](128);
}
