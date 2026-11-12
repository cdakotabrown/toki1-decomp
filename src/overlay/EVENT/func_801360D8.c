/* @SymbolStart: 0x801360D0 */
/* @SymbolSize: 196 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=10:a0,7:a1,40:a2,41:a3,42:v1 */
/* Address: 0x801360D8 */
/* Size: 196 bytes (49 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern void func_8004C984(void);

extern u8 D_800F19AB;
extern u8 D_800EC190[];
extern u8 D_800B1AF6;

void func_801360D8(void) {
    s32 pad[2];
    u32 arg0_v = D_800B1AF6;

    s16 i;
    u8 v;

    func_8004C984();
    v = D_800F19AB;
    for (i = 4; i < 19; i++) {
        if ((u32)v < 96) {
            D_800EC190[i * 36 + 22] = v;
            D_800EC190[i * 36 + 21] = v;
            D_800EC190[i * 36 + 20] = v;
        }
    }
    if (arg0_v == D_800B1AF6) {
        return;
    }
    for (i = 4; i < 19; i++) {
        *(s32 *)&D_800EC190[i * 36] |= 0x80000000;
    }
}
