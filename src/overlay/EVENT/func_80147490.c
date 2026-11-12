/* Address: 0x80147490 */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable;

extern u8 D_800B1AF6;
extern u8 D_8012411C;
extern u16 D_80094714;
extern void *D_8011FB6C;
extern JumpTable D_80124120;

extern void func_80078950();
extern void func_80033E88(void);

void func_80147490(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80124120;

    func_80078950(&D_8011FB6C, D_800B1AF6, D_80094714);

    table.ptrs[D_800B1AF6](128);

    if (D_8012411C != 0) {
        func_80033E88();
    }
}
