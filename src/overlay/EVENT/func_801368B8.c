/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* Address: 0x801368B8 */
/* Size: 336 bytes (83 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern s16 D_80094714;
extern s16 D_80094718;
extern s16 D_800B0A06;
extern s16 D_800B0A0A;
extern s32 D_800EECB0;
extern s32 D_801202D0;
extern s32 D_801202F0;
extern s32 D_80120310;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern u8 D_8011F5D0[];
extern u8 D_8011F5D4[];
extern u8 D_80094784[];

extern void func_80012D2C(s32 a, s32 b);
extern void func_8001886C(s32 a);
extern void func_80010678(void);
extern void func_80018944(s32 a);
extern void func_80012D40(s32 a);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_80036884(u8 *p);
extern void func_8004CDDC(void);
extern void func_80045B14(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_8004BC20(s32 a);
extern void func_800F6000(void);
extern void func_80078970(u8 *a, u8 *b);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801368B8(void) {
    D_800B1746 = 0;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011F5D0);
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_8004CDDC();
    func_80045B14();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    func_8004BC20(D_800B1746);
    func_800F6000();
    D_80120678 = D_801202D0;
    D_8012067C = D_801202F0;
    D_80120680 = D_80120310;
    func_80078970(D_80094784, D_8011F5D4);
    if (D_800EECB0 == 1) {
        D_800B0A06 = (s16)(D_800B0A06 + 1);
        D_800B0A0A = (s16)(D_800B0A0A + 2);
        func_8004C250();
    }
    func_80011DFC();
}
