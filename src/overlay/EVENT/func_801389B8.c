/* func_801389B8 -- EVENT overlay, Address: 0x801389B8 */
/* @SymbolStart: 0x80138994 */
/* @SymbolSize: 488 */
#include "types.h"

extern s32 D_8012037C;
extern s32 D_80120394;
extern s32 D_801203AC;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80121288;
extern s32 D_8012128C;
extern s32 D_80121290;
extern s16 D_80121294;
extern s32 D_80121298;
extern s32 D_8012129C;
extern s32 D_801212A0;
extern s32 D_801212A4;
extern s32 D_801212A8;
extern s32 D_801212AC;
extern s32 D_801212B0;
extern u8  D_800EB029;
extern u8  D_800EB02A;
extern u8  D_800EB02B;
extern u8  D_800EB02C;
extern u8  D_800EB02D;
extern u8  D_800EB02E;
extern u8  D_800EB02F;
extern s32 D_800EB034;
extern s32 D_800EB038;
extern s16 D_800EB03C;
extern s16 D_800EB03E;
extern s16 D_800EB040;
extern s16 D_800EB04E;
extern s16 D_800EB052;
extern s32 D_800EB05C;
extern s32 D_800EB060;
extern u8  D_800EB06B;
extern volatile s16 D_800B0A6E;
extern volatile s16 D_800B0A72;
extern volatile s16 D_800B0A76;

extern void func_800FBE90(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_800189C4(s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801389B8(void) {
    D_80120678 = D_8012037C;
    D_8012067C = D_80120394;
    D_80120680 = D_801203AC;
    func_800FBE90();
    func_80012D64(D_80121298, 17, 1, 2, 0);
    func_8004C46C(D_8012129C, D_801212A0, D_801212A4, D_801212A8,
                  D_801212AC, D_801212B0);
    func_8004C6B0(D_8012128C, D_80121290, D_80121288, D_80121294);
    func_800189C4(98);
    D_800EB029 = 9;
    D_800EB02A = 1;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_8012128C;
    D_800EB038 = D_80121290;
    D_800EB05C = D_80121288;
    D_800EB03C = D_80121294;
    D_800EB03E = 4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 17;
    D_800EB04E = -160;
    D_800EB052 = -120;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_800EB02C = 0;
    D_800B0A6E = (s16)(D_800B0A6E + 3);
    D_800B0A72 = (s16)(D_800B0A72 + 2);
    D_800B0A76 = (s16)(D_800B0A76 - 20);
    func_8004C250();
    func_80011DFC();
}
