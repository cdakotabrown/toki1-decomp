/* func_8013D9C8 -- EVENT overlay, Address: 0x8013D9C8 */
/* @SymbolStart: 0x8013D9A4 */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_80120338;
extern s32 D_8012034C;
extern s32 D_80120360;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0A3A;
extern volatile s16 D_800B0A3E;
extern volatile s16 D_800B0A42;
extern s32 D_80122228;
extern s32 D_8012222C;
extern s32 D_80122230;
extern s16 D_80122234;
extern s32 D_80122238;
extern s32 D_8012223C;
extern s32 D_80122240;
extern s32 D_80122244;
extern s32 D_80122248;
extern s32 D_8012224C;
extern s32 D_80122250;

extern void func_80101300(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013D9C8(void) {
    D_80120678 = D_80120338;
    D_8012067C = D_8012034C;
    D_80120680 = D_80120360;
    func_80101300();
    func_80012D64(D_80122238, 17, 1, 2, 0);
    func_8004C46C(D_8012223C, D_80122240, D_80122244, D_80122248,
                  D_8012224C, D_80122250);
    func_8004C6B0(D_8012222C, D_80122230, D_80122228, D_80122234);
    D_800B0A3A = (s16)(D_800B0A3A + 1);
    D_800B0A3E = (s16)(D_800B0A3E + 2);
    D_800B0A42 = (s16)(D_800B0A42 - 20);
    func_8004C250();
    func_80011DFC();
}
