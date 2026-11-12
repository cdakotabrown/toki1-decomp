/* @PostProc: patch_stack_complete --old 208 --new 224 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8013767C */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[45];
} JumpTable;

extern u8 D_800B1AF6;
extern JumpTable D_80120EE0;

void func_8013767C(void) {
    JumpTable table;

    table = D_80120EE0;
    table.ptrs[D_800B1AF6](128);
}
