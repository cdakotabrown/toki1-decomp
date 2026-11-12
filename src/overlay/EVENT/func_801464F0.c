/* Address: 0x801464F0 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[22];
} JumpTable;

extern u8 D_800B1AF6;
extern u16 D_80094714;
extern void *D_8011FAF4;
extern JumpTable D_80123ED8;

extern void func_80078950();

void func_801464F0(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80123ED8;

    func_80078950(&D_8011FAF4, D_800B1AF6, D_80094714);

    table.ptrs[D_800B1AF6]();
}
