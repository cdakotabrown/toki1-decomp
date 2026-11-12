/* @SymbolStart: 0x80154690 */
/* @SymbolSize: 212 */
/* @PostProc: patch_insn_reg_field 0x50:rt:v1:v0 0x5c:rs:v1:v0 0x5c:rt:v1:v0 0x64:rs:v1:v0 0x68:rs:v1:v0 */
/* Address: 0x8015469C */
/* Size: 200 bytes (50 instructions) */
#include "types.h"

extern s32 D_80094730;
extern u8 D_800B1746;
extern s32 D_800B0940;
extern u8 D_8012531C;
extern s16 D_80094734;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s32 D_800EECBC;

extern s32 func_80049B20(s32, s32, s32);
extern void func_80011DFC(void);

void func_8015469C(void) {
    s32 v;

    D_8009471C = (s32)&D_80094730;
    if (D_800B1746 == 0) {
        if (((u32)D_800B0940 >> 28) == 6) {
            if (D_8012531C == 0) {
                v = D_80094734;
                if (v == 0) {
                    D_80094734 = v + 1;
                }
            }
        }
    }
    D_80094720 = (s32)&D_80094734;
    D_80094724 = D_8009473C;
    D_80094728 = D_80094740;
    D_8009472C = D_80094744;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}
