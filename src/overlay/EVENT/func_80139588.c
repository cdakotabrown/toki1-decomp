/* @PostProc: patch_insn_reg_field 0x20:rt:v0:v1 0x24:rs:v0:v1 0x24:rt:v0:v1 0x2C:rs:v0:v1 0x44:rt:v0:v1 0x48:rs:v0:v1 0x48:rt:v0:v1 0x50:rs:v0:v1 */
/* Address: 0x80139588 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

extern void func_800F7074(void);
extern void func_800433D0(s32);
extern void func_80011DFC(void);
extern u32 D_800B1AE4;

void func_80139588(void)
{
    u32 t;

    func_800F7074();
    t = D_800B1AE4;
    if ((t % 60) == 0) {
        func_800433D0(1284);
    }
    t = D_800B1AE4;
    if ((t % 60) == 30) {
        func_800433D0(1283);
    }
    func_80011DFC();
}
