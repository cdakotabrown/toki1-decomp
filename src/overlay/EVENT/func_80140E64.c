/* Address: 0x80140E64 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable;

extern u8 D_800B1AF6;
extern char D_8011F928;
extern JumpTable D_80122E24;

extern void func_80078950(char *s, u8 a1);

void func_80140E64(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80122E24;

    func_80078950(&D_8011F928, D_800B1AF6);
    table.ptrs[D_800B1AF6](128);
}
