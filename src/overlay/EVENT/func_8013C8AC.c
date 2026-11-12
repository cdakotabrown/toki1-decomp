/* func_8013C8AC -- EVENT overlay, Address: 0x8013C8AC */
/* @SymbolStart: 0x8013C888 */
/* @SymbolSize: 280 */
#include "types.h"

extern s32 D_801204E0;
extern s32 D_801204FC;
extern s32 D_80120518;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_800947C4;
extern s32 D_8011F7B8;
extern volatile s16 D_800B0B76;
extern volatile s16 D_800B0B7A;
extern s32 D_80121DA4;
extern s32 D_80121DA8;
extern s32 D_80121DAC;
extern s16 D_80121DB0;
extern s32 D_80121DB4;
extern s32 D_80121DB8;
extern s32 D_80121DBC;
extern s32 D_80121DC0;
extern s32 D_80121DC4;
extern s32 D_80121DC8;
extern s32 D_80121DCC;

extern void func_80078970(s32 *a, s32 *b);
extern void func_8004C250(void);
extern void func_800FFDF0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_8013C8AC(void) {
    D_80120678 = D_801204E0;
    D_8012067C = D_801204FC;
    D_80120680 = D_80120518;
    func_80078970(&D_800947C4, &D_8011F7B8);
    D_800B0B76 = (s16)(D_800B0B76 - 1);
    D_800B0B7A = (s16)(D_800B0B7A + 10);
    func_8004C250();
    func_800FFDF0();
    func_80012D64(D_80121DB4, 17, 1, 2, 0);
    func_8004C46C(D_80121DB8, D_80121DBC, D_80121DC0, D_80121DC4,
                  D_80121DC8, D_80121DCC);
    func_8004C6B0(D_80121DA8, D_80121DAC, D_80121DA4, D_80121DB0);
    func_80011DFC();
}
