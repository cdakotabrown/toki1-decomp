/* func_8013A4B8 -- EVENT overlay, Address: 0x8013A4B8 */
/* @SymbolStart: 0x8013A494 */
/* @SymbolSize: 556 */
#include "types.h"

extern s32 D_801203BC;
extern s32 D_801203D4;
extern s32 D_801203EC;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80121858;
extern s32 D_8012185C;
extern s32 D_80121860;
extern s16 D_80121864;
extern s32 D_80121868;
extern s32 D_8012186C;
extern s32 D_80121870;
extern s32 D_80121874;
extern s32 D_80121878;
extern s32 D_8012187C;
extern s32 D_80121880;

extern u8 D_800EB029;
extern u8 D_800EB02A;
extern u8 D_800EB02B;
extern u8 D_800EB02C;
extern u8 D_800EB02D;
extern u8 D_800EB02E;
extern u8 D_800EB02F;
extern s16 D_800EB03C;
extern s16 D_800EB03E;
extern s16 D_800EB040;
extern s16 D_800EB04E;
extern s16 D_800EB052;
extern s32 D_800EB034;
extern s32 D_800EB038;
extern s32 D_800EB05C;
extern s32 D_800EB060;
extern u8 D_800EB06B;

extern volatile s16 D_800B0AA2;
extern volatile s16 D_800B0AA6;
extern volatile s16 D_800B0AAA;

extern s16 D_80124394;
extern s16 D_80124396;
extern s16 D_80124398;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern s16 D_801243A8;

extern void func_800FDD80(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_800189C4(s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013A4B8(void) {
    D_80120678 = D_801203BC;
    D_8012067C = D_801203D4;
    D_80120680 = D_801203EC;
    func_800FDD80();
    func_80012D64(D_80121868, 17, 1, 2, 0);
    func_8004C46C(D_8012186C, D_80121870, D_80121874, D_80121878,
                  D_8012187C, D_80121880);
    func_8004C6B0(D_8012185C, D_80121860, D_80121858, D_80121864);
    func_800189C4(98);
    D_800EB029 = 9;
    D_800EB02A = 1;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_8012185C;
    D_800EB038 = D_80121860;
    D_800EB05C = D_80121858;
    D_800EB03C = (s16)D_80121864;
    D_800EB03E = 4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 17;
    D_800EB04E = -160;
    D_800EB052 = -120;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_800EB02C = 0;
    D_800B0AA2 = (s16)(D_800B0AA2 - 1);
    D_800B0AA6 = (s16)(D_800B0AA6 - 1);
    D_800B0AAA = (s16)(D_800B0AAA + 20);
    func_8004C250();
    D_80124394 = 3;
    D_80124396 = 3;
    D_80124398 = 3;
    D_801243A4 = 2;
    D_801243A6 = 2;
    D_801243A8 = 2;
    func_80011DFC();
}
