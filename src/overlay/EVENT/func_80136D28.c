/* Address: 0x80136D28 */
/* Size: 324 bytes (81 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern s32 D_800B1AE4;
extern s16 D_80094714;
extern s16 D_80094718;
extern s32 D_801202EC;
extern s32 D_8012030C;
extern s32 D_8012032C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_800B1C70;
extern s32 D_800B1CF0;
extern u8 D_8011F5E8[];
extern u8 D_8011F5EC[];
extern u8 D_8011F5F8[];
extern u8 D_80094784[];
extern u8 D_800947C4[];

extern void func_8011F530(s32 a);
extern void func_80012D2C(s32 a, s32 b);
extern void func_8001886C(s32 a);
extern void func_80010678(void);
extern void func_80018944(s32 a);
extern void func_80012D40(s32 a);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_80036884(u8 *p);
extern void func_8004CDDC(void);
extern void func_80078970(u8 *a, u8 *b);
extern void func_80045B14(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_8004BC20(s32 a);
extern void func_800F6000(void);
extern void func_80011DFC(void);

void func_80136D28(void) {
    func_8011F530(D_800B1AE4);
    D_800B1746 = 0;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011F5E8);
    D_800E96EF = 0;
    D_800E9733 = 0;
    func_8004CDDC();
    func_80078970(D_80094784, D_8011F5EC);
    func_80078970(D_800947C4, D_8011F5F8);
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    func_8004BC20(D_800B1746);
    func_800F6000();
    D_80120678 = D_801202EC;
    D_8012067C = D_8012030C;
    D_80120680 = D_8012032C;
    D_800B1C70 = -1;
    D_800B1CF0 = -1;
    func_80011DFC();
}
