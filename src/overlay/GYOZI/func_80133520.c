/* @SymbolStart: 0x80133518 */
/* func_80133520 -- GYOZI overlay, Address: 0x80133518 (head) / 0x80133520, */
/* Size: 272 bytes (68 instructions). */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 */
/* @PostProc: swap_branch_operands --offsets 0x44 */
/* @SymbolSize: 272 */
#include "types.h"
extern void func_80135730(void);
extern s32 func_8004DE1C(void);
extern void func_8008F618(s32 a0);
extern void func_80134938(void);
extern u8 D_800F555B[];
extern u8 D_800F62CF;
extern s32 D_80145E34;
extern s32 D_80145E6C;
extern s32 D_80145EA4;
extern s32 D_80145EA8;
extern s32 D_80145EAC;
extern s32 D_80145EB0;
extern s16 D_80145EB4;
extern u8 D_80145EC4;
extern s32 D_8012E66C;

s32 func_80133520(void)
{
    s32 v;

    if (D_8012E66C == 11) {
        func_80135730();
        return 0;
    }
    v = D_8012E66C;
    if (D_800F62CF == D_8012E66C && D_80145EC4 != 0) {
        func_8004DE1C();
        return 0;
    }
    if (D_800F555B[D_8012E66C * 56] == 0) {
        D_80145EB4 = 0;
        D_80145EA8 = D_80145E34;
        D_80145EAC = D_80145E6C;
        D_80145EB0 = D_80145EA4;
        func_8008F618(D_8012E66C);
        func_8004DE1C();
        return 0;
    }
    D_800F62CF = v;
    func_80134938();
    func_8008F618(D_800F62CF);
    D_80145EB4 = 9;
    func_8004DE1C();
    func_8004DE1C();
    return func_8004DE1C();
}
