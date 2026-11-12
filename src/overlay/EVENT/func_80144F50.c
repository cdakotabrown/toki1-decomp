/* Address: 0x80144F50 */
/* Size: 388 bytes (97 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern s8 D_800E96EF;
extern s8 D_800E9733;
extern s16 D_80094714;
extern s16 D_80094718;
extern s8 D_800F19AB;
extern s32 D_801205C4;
extern s32 D_801205E0;
extern s32 D_801205FC;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_800EECB0;
extern volatile s16 D_800B0C0E;
extern volatile s16 D_800B0C12;
extern volatile s16 D_800B094E;
extern volatile s16 D_800B096A;
extern u8 D_8011FA5C[];
extern u8 D_80094784[];
extern u8 D_8011FA60[];

extern void func_80012D2C(s32, s32);
extern void func_8001886C(s32);
extern void func_80010678(void);
extern void func_80018944(s32);
extern void func_80012D40(s32);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_80036884(void *);
extern void func_80045B14(void);
extern void func_800F6000(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_8004BC20(s32);
extern void func_8004CDDC(void);
extern void func_80078970(void *, void *);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_80144F50(void) {
    D_800B1746 = 10;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011FA5C);
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_800F6000();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_801205C4;
    D_8012067C = D_801205E0;
    D_80120680 = D_801205FC;
    func_80078970(D_80094784, D_8011FA60);
    if (D_800EECB0 == 1) {
        D_800B0C0E = D_800B0C0E + 1;
        D_800B0C12 = D_800B0C12 + 2;
        D_800B094E = D_800B094E - 5;
        D_800B096A = D_800B096A + 5;
        func_8004C250();
    }
    func_80011DFC();
}
