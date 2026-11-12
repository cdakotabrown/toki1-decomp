/* @PostProc: patch_stack_complete --old 184 --new 208 */
/* @PostProc: remap_stack_slots --from 28 --to 48 */
/* @PostProc: patch_insn_reg_field 0x90:rt:v0:v1 0x94:rt:v0:v1 0x94:rs:v0:v1 0x9C:rs:v0:v1 0xB4:rt:v0:v1 0xB8:rt:v0:v1 0xB8:rs:v0:v1 0xC0:rs:v0:v1 */
/* Address: 0x80138DD8 */
/* Size: 228 bytes (57 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[39];
} JumpTable;

extern u8 D_800B1AF6;
extern u32 D_800B1AE4;
extern JumpTable D_80121404;

extern void func_800FCFA4(void);
extern void func_800FCFC4(void);

void func_80138DD8(void)
{
    JumpTable table;

    table = D_80121404;

    table.ptrs[D_800B1AF6](128);

    if ((u32)D_800B1AF6 >= 13U && (u32)D_800B1AF6 < 16U) {
        if ((D_800B1AE4 % 180U) == 0) {
            func_800FCFA4();
        }
        if ((D_800B1AE4 % 180U) == 90) {
            func_800FCFC4();
        }
    }
}
