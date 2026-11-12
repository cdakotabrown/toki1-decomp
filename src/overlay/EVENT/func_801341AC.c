/* func_801341AC -- EVENT overlay, Address: 0x801341AC */
/* @SymbolStart: 0x80134188 */
/* @SymbolSize: 296 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
#include "types.h"

extern s32 D_8012053C;
extern s32 D_80120554;
extern s32 D_8012056C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_8012077C;
extern s32 D_80120780;
extern s32 D_80120784;
extern s32 D_80120788;
extern s32 D_8012078C;
extern s32 D_80120790;
extern s32 D_80120794;
extern s32 D_8012076C;
extern s32 D_80120770;
extern s32 D_80120774;
extern s16 D_80120778;
extern volatile s16 D_800B0BA6;
extern volatile s16 D_800B0BAA;
extern volatile s16 D_800B0BAE;
extern u8 D_800EAFA4;
extern u8 D_800EAFE8;

extern void func_800F7590(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801341AC(void) {
    D_80120678 = D_8012053C;
    D_8012067C = D_80120554;
    D_80120680 = D_8012056C;
    func_800F7590();
    func_80012D64(D_8012077C, 17, 1, 2, 0);
    func_8004C46C(D_80120780, D_80120784, D_80120788, D_8012078C,
                  D_80120790, D_80120794);
    func_8004C6B0(D_80120770, D_80120774, D_8012076C, D_80120778);
    D_800EAFA4 = 8;
    D_800B0BA6 = (s16)(D_800B0BA6 + 1);
    D_800B0BAA = (s16)(D_800B0BAA + 1);
    D_800B0BAE = (s16)(D_800B0BAE - 20);
    D_800EAFE8 = 8;
    func_8004C250();
    func_80011DFC();
}
