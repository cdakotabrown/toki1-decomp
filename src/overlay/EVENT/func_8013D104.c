/* Address: 0x8013D104 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[15];
} JumpTable;

extern u8 D_800B1AF6;
extern u16 D_80094714;
extern u16 D_80094718;
extern char D_8011F7DC;
extern JumpTable D_80122194;

extern void func_80078950(char *s, u8 a1, u16 a2, u16 a3);

void func_8013D104(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80122194;

    func_80078950(&D_8011F7DC, D_800B1AF6, D_80094714, D_80094718);
    table.ptrs[D_800B1AF6](128);
}
