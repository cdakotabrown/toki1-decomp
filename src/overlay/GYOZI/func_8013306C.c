/* @SymbolSize: 172 */
/* @CompileFlags: -cckr */
#include "types.h"

extern s32 D_800F6474;
extern s32 **D_80145EA8;
extern u8 **D_80145EAC;
extern s16 D_80145EB4;
extern s16 D_80145EB8;
extern s32 D_801458E0;

func_8013306C()
{
    func_801343A4();
    if (D_800F6474++ == 0) {
        if (D_80145EAC[D_80145EB4][D_80145EB8 - 1] == 7) {
            if (func_800BDC20(D_80145EA8[D_80145EB4][D_80145EB8 - 1], &D_801458E0) == 0) {
                func_80086AB0(1548);
            }
        }
    }
}
