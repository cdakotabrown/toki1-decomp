/* Address: 0x80141C8C */
/* Size: 364 bytes (91 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern u8 D_800F19AB;
extern s16 D_80094714;
extern s16 D_80094718;
extern s16 D_800B0BDA;
extern s16 D_800B0BDE;
extern s32 D_800EECB0;
extern s32 D_80120570;
extern s32 D_8012058C;
extern s32 D_801205A8;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern u8 D_8011F968[];
extern u8 D_8011F96C[];
extern u8 D_80094784[];

extern void func_80012D2C(s32 a, s32 b);
extern void func_8001886C(s32 a);
extern void func_80010678(void);
extern void func_80018944(s32 a);
extern void func_80012D40(s32 a);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_80036884(u8 *p);
extern void func_80045B14(void);
extern void func_800F6000(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_8004BC20(s32 a);
extern void func_8004CDDC(void);
extern void func_80078970(u8 *a, u8 *b);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_80141C8C(void) {
    D_800B1746 = 9;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011F968);
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
    D_80120678 = D_80120570;
    D_8012067C = D_8012058C;
    D_80120680 = D_801205A8;
    func_80078970(D_80094784, D_8011F96C);
    if (D_800EECB0 == 1) {
        if (D_800B0BDE >= 80) {
            D_800B0BDA = (s16)(D_800B0BDA + 1);
        } else {
            D_800B0BDE = (s16)(D_800B0BDE + 1);
        }
        func_8004C250();
    }
    func_80011DFC();
}
