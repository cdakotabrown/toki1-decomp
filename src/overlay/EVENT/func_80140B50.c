/* Address: 0x80140B50 */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[55];
} JumpTable;

extern u8 D_800B1AF6;
extern void *D_8011F924;
extern JumpTable D_80122D48;

extern void func_80078950();

void func_80140B50(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80122D48;

    func_80078950(&D_8011F924, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);
}
