/* @SymbolStart: 0x80153CEC */
/* @SymbolSize: 148 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --map '26:46,30:20' */
/* Address: 0x80153CF4 */
/* Size: 148 bytes (37 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s16 D_8012439C[];
extern s32 D_801241D8;
extern s32 D_801241E8;
extern s32 D_801241F8;

extern s32 func_80049B20();
extern void func_80011DFC();

void func_80153CF4(void) {
    s32 idx_v = D_800EECBC;

    s16 local;

    D_8009471C = (s32)&local;
    D_80094720 = (s32)&D_8012439C[idx_v];
    D_80094724 = D_801241D8;
    D_80094728 = D_801241E8;
    D_8009472C = D_801241F8;
    local = 0;
    if (func_80049B20(idx_v, 0, 0) != 0) {
        func_80011DFC();
    }
}
