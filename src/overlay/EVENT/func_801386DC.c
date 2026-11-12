/* func_801386DC -- EVENT overlay, Address: 0x801386DC */
/* @SymbolStart: 0x801386B8 */
/* @SymbolSize: 732 */
#include "types.h"

extern s32 D_80120378;
extern s32 D_80120390;
extern s32 D_801203A8;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80121254;
extern s32 D_80121258;
extern s32 D_8012125C;
extern s16 D_80121260;
extern s32 D_80121264;
extern s32 D_80121268;
extern s32 D_8012126C;
extern s32 D_80121270;
extern s32 D_80121274;
extern s32 D_80121278;
extern s32 D_8012127C;
extern s32 D_80121280;
extern s32 D_80121284;

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

extern s32 D_800B1C70;
extern volatile u8 D_800B1897;

extern s32 D_800EC2F8;
extern s16 D_800EC2FC;
extern s16 D_800EC2FE;
extern s16 D_800EC300;
extern s16 D_800EC302;
extern s16 D_800EC304;
extern u8 D_800EC306;
extern u8 D_800EC307;
extern s16 D_800EC308;
extern s16 D_800EC30A;
extern u8 D_800EC30C;
extern u8 D_800EC30D;
extern u8 D_800EC30E;
extern s16 D_800EC310;
extern s16 D_800EC312;
extern s16 D_800EC314;
extern s16 D_800EC316;
extern s32 D_800EC318;

extern volatile s16 D_800B0A72;
extern volatile s16 D_800B0A76;

extern void func_800FBDC0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_800189C4(s32);
extern void func_800137AC(s32, s32, s32, s32, s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801386DC(void) {
    D_80120678 = D_80120378;
    D_8012067C = D_80120390;
    D_80120680 = D_801203A8;
    func_800FBDC0();
    func_80012D64(D_80121264, 17, 1, 2, 0);
    func_8004C46C(D_8012126C, D_80121270, D_80121274, D_80121278,
                  D_8012127C, D_80121280);
    func_8004C6B0(D_80121258, D_8012125C, D_80121254, D_80121260);
    func_800189C4(98);
    D_800EB029 = 9;
    D_800EB02A = 1;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_80121258;
    D_800EB038 = D_8012125C;
    D_800EB05C = D_80121254;
    D_800EB03C = (s16)D_80121260;
    D_800EB03E = 4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 17;
    D_800EB04E = -160;
    D_800EB052 = -120;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_800EB02C = 0;
    func_80012D64(D_80121268, 18, 1, 0, 0);
    D_800B1C70 = 1;
    func_800137AC(640, 256, 64, 128, D_80121284);
    D_800EC2F8 = 0x81000000;
    D_800EC300 = 85;
    D_800EC302 = 56;
    D_800EC306 = 0;
    D_800EC307 = 0;
    D_800EC308 = 0;
    D_800EC304 = 26;
    D_800EC30A = 498;
    D_800EC30C = 0;
    D_800EC30D = 0;
    D_800EC30E = 0;
    D_800EC310 = 0;
    D_800EC312 = 0;
    D_800EC314 = 4096;
    D_800EC316 = 4096;
    D_800EC318 = 0;
    D_800EC2FC = 5;
    D_800EC2FE = -30;
    D_800B1897 = 11;
    D_800B0A72 = (s16)(D_800B0A72 + 2);
    D_800B0A76 = (s16)(D_800B0A76 - 10);
    func_8004C250();
    func_80011DFC();
}
