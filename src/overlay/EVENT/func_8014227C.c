/* func_8014227C -- EVENT overlay, Address: 0x8014227C */
/* @SymbolStart: 0x80142258 */
/* @SymbolSize: 300 */
#include "types.h"

extern s32 D_80120578;
extern s32 D_80120594;
extern s32 D_801205B0;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80094784;
extern s32 D_8011F978;
extern volatile s16 D_800B0BDA;
extern volatile s16 D_800B0BDE;
extern volatile s16 D_800B0BE2;
extern s32 D_80122F9C;
extern s32 D_80122FA0;
extern s32 D_80122FA4;
extern s16 D_80122FA8;
extern s32 D_80122FAC;
extern s32 D_80122FB0;
extern s32 D_80122FB4;
extern s32 D_80122FB8;
extern s32 D_80122FBC;
extern s32 D_80122FC0;
extern s32 D_80122FC4;

extern void func_80078970(s32 *a, s32 *b);
extern void func_8004C250(void);
extern void func_801051A0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_8014227C(void) {
    D_80120678 = D_80120578;
    D_8012067C = D_80120594;
    D_80120680 = D_801205B0;
    func_80078970(&D_80094784, &D_8011F978);
    D_800B0BDA = (s16)(D_800B0BDA + 1);
    D_800B0BDE = (s16)(D_800B0BDE + 1);
    D_800B0BE2 = (s16)(D_800B0BE2 - 20);
    func_8004C250();
    func_801051A0();
    func_80012D64(D_80122FAC, 17, 1, 2, 0);
    func_8004C46C(D_80122FB0, D_80122FB4, D_80122FB8, D_80122FBC,
                  D_80122FC0, D_80122FC4);
    func_8004C6B0(D_80122FA0, D_80122FA4, D_80122F9C, D_80122FA8);
    func_80011DFC();
}
