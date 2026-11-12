/* @SymbolStart: 0x80152888 */
/* @SymbolSize: 184 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* Address: 0x80152890 */
/* Size: 184 bytes (46 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_80125328;

extern void func_80011DFC(void);

extern u8 D_800B1746;
extern s16 D_80094730;
extern u8 D_800B1AF6;

void func_80152890(void) {
    u8 step_v = D_80125328;

    s32 id;

    if (step_v == 0) {
        id = D_800B1746;
        D_80094730 = 9;
        if (id == 7) {
            D_80094730 = 10;
        } else if (id == 2 || id == 9) {
            D_80094730 = 8;
        } else if (id == 4 || id == 8 || id == 5 || id == 10) {
            D_80094730 = 7;
        }
        D_800B1AF6 = D_800B1AF6 + 20;
    }

    func_80011DFC();
}
