/* Address: 0x8013EC0C */
/* Size: 184 bytes (46 instructions) */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
#include "types.h"

extern s16 D_800B0BAE;

extern s8 D_800EAFA4;
extern s8 D_800EAFE8;
extern s32 D_80122680;
extern s32 D_80122684;
extern s32 D_80122688;
extern s16 D_8012268C;
extern s32 D_80122690;
extern s32 D_80122694;
extern s32 D_80122698;
extern s32 D_8012269C;
extern s32 D_801226A0;
extern s32 D_801226A4;
extern s32 D_801226A8;

extern void func_801024B0(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_8013EC0C(void) {
    s8 val;

    func_801024B0();
    func_80012D64(D_80122690, 17, 1, 2, 0);
    func_8004C46C(D_80122694, D_80122698, D_8012269C, D_801226A0, D_801226A4, D_801226A8);
    func_8004C6B0(D_80122684, D_80122688, D_80122680, D_8012268C);
    val = 8;
    D_800EAFA4 = val;
    D_800EAFE8 = val;
    func_80011DFC();
}
