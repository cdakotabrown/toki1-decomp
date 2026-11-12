/* @SymbolSize: 156 */
/* @PostProc: patch_stack_complete --old 240 --new 264 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80139FD4, Size: 156 bytes (39 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[54];
} JumpTable;

extern u8 D_800F647A;
extern s16 D_801474A8;
extern s16 D_801474AC;
extern s32 D_8012E684;
extern JumpTable D_801479F8;

void func_80139FD4(void)
{
    JumpTable table;

    table = D_801479F8;

    table.ptrs[D_800F647A](128);

    if (D_801474A8 == 0) {
        if (D_801474AC == 3) {
            D_8012E684 = 0;
        }
    }
}
