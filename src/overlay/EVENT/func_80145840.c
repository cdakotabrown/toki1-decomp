/* func_80145840 -- EVENT overlay, Address: 0x80145840 */
/* @SymbolStart: 0x8014581C */
/* @SymbolSize: 300 */
#include "types.h"

extern s32 D_801205D0;
extern s32 D_801205EC;
extern s32 D_80120608;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_800947C4;
extern s32 D_8011FA80;
extern volatile s16 D_800B0C0E;
extern volatile s16 D_800B0C12;
extern volatile s16 D_800B0C16;
extern s32 D_801238D8;
extern s32 D_801238DC;
extern s32 D_801238E0;
extern s16 D_801238E4;
extern s32 D_801238E8;
extern s32 D_801238EC;
extern s32 D_801238F0;
extern s32 D_801238F4;
extern s32 D_801238F8;
extern s32 D_801238FC;
extern s32 D_80123900;

extern void func_80078970(s32 *a, s32 *b);
extern void func_8004C250(void);
extern void func_80108120(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80145840(void) {
    D_80120678 = D_801205D0;
    D_8012067C = D_801205EC;
    D_80120680 = D_80120608;
    func_80078970(&D_800947C4, &D_8011FA80);
    D_800B0C0E = (s16)(D_800B0C0E + 3);
    D_800B0C12 = (s16)(D_800B0C12 + 2);
    D_800B0C16 = (s16)(D_800B0C16 - 20);
    func_8004C250();
    func_80108120();
    func_80012D64(D_801238E8, 17, 1, 2, 0);
    func_8004C46C(D_801238EC, D_801238F0, D_801238F4, D_801238F8,
                  D_801238FC, D_80123900);
    func_8004C6B0(D_801238DC, D_801238E0, D_801238D8, D_801238E4);
    func_80011DFC();
}
