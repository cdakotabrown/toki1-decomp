/* @SymbolStart: 0x80156508 */
/* @SymbolSize: 152 */
/* Address: 0x80156510 */
/* Size: 152 bytes (38 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern void func_8004CF30(void);
extern void func_80011DFC(void);

extern s32 D_800EECBC;
extern u8 D_80125320;
extern u8 D_8012531C;
extern s32 D_800B1AF0;
extern s16 D_80094730;

void func_80156510(void) {
    u8 step_v = D_800B1746;

    s32 count;
    s32 first;

    if (step_v == 10 && (s32)D_800EECBC - 1 == 0 && D_80125320 == 2 &&
            (s32)D_8012531C - 1 == 0) {
        count = D_800B1AF0;
        first = (u32)count < 1;
        D_800B1AF0 = count + 1;
        if (first) {
            D_80094730 = 55;
        }
        func_8004CF30();
    } else {
        func_80011DFC();
    }
}
