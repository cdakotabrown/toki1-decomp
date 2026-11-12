/* @SymbolSize: 156 */
/* Address: 0x801400F0, Size: 156 bytes (39 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[29];
} JumpTable;

extern u8 D_800B1AF6;
extern u8 D_800E9E63;
extern u8 D_8011F90C[];
extern JumpTable D_80122C0C;
extern void func_80078950(u8 *, u8, u8);

void func_801400F0(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80122C0C;

    func_80078950(D_8011F90C, D_800B1AF6, D_800E9E63);

    table.ptrs[D_800B1AF6]();
}
