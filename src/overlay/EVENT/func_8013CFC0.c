/* Address: 0x8013CFC0 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable;

extern u8 D_800B1AF6;
extern u16 D_80094714;
extern u16 D_80094718;
extern char D_8011F7D0;
extern JumpTable D_801220EC;

extern void func_80078950(char *s, u8 a1, u16 a2, u16 a3);

void func_8013CFC0(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_801220EC;

    func_80078950(&D_8011F7D0, D_800B1AF6, D_80094714, D_80094718);
    table.ptrs[D_800B1AF6](128);
}
