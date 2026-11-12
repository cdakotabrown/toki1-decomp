/* func_8013A6E4 -- EVENT overlay, Address: 0x8013A6E4 */
/* @SymbolStart: 0x8013A6C0 */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_801203C0;
extern s32 D_801203D8;
extern s32 D_801203F0;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0AA2;
extern volatile s16 D_800B0AA6;
extern volatile s16 D_800B0AAA;
extern s32 D_80121884;
extern s32 D_80121888;
extern s32 D_8012188C;
extern s16 D_80121890;
extern s32 D_80121894;
extern s32 D_80121898;
extern s32 D_8012189C;
extern s32 D_801218A0;
extern s32 D_801218A4;
extern s32 D_801218A8;
extern s32 D_801218AC;

extern void func_800FDE30(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013A6E4(void) {
    D_80120678 = D_801203C0;
    D_8012067C = D_801203D8;
    D_80120680 = D_801203F0;
    func_800FDE30();
    func_80012D64(D_80121894, 17, 1, 2, 0);
    func_8004C46C(D_80121898, D_8012189C, D_801218A0, D_801218A4,
                  D_801218A8, D_801218AC);
    func_8004C6B0(D_80121888, D_8012188C, D_80121884, D_80121890);
    D_800B0AA2 = (s16)(D_800B0AA2 + 2);
    D_800B0AA6 = (s16)(D_800B0AA6 + 1);
    D_800B0AAA = (s16)(D_800B0AAA - 20);
    func_8004C250();
    func_80011DFC();
}
