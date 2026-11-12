/* Address: 0x80146AF8 */
/* Size: 328 bytes (82 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800F19AB;
extern s16 D_80094714;
extern s16 D_80094718;
extern s32 D_80120630;
extern s32 D_80120650;
extern s32 D_80120670;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern u8 D_80094748[];
extern u8 D_800B0929[];
extern u8 D_80094764[];
extern u8 D_8011FB14[];
extern u8 D_8009476C[];
extern u8 D_8011FB1C[];
extern u8 D_80094784[];
extern u8 D_8011FB28[];
extern u8 D_800947C4[];
extern u8 D_8011FB3C[];

extern void func_80012D2C(s32 a, s32 b);
extern void func_8001886C(s32 a);
extern void func_80010678(void);
extern void func_80018944(s32 a);
extern void func_80012D40(s32 a);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_800201FC(s32 a);
extern void func_80045B14(void);
extern void func_800F6000(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_80078970(u8 *a, u8 *b);
extern void func_8004CDDC(void);
extern void func_80011DFC(void);

void func_80146AF8(void) {
    D_800B1746 = 11;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_800201FC(1);
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_80078970(D_80094748, D_800B0929);
    func_80078970(D_80094764, D_8011FB14);
    func_80078970(D_8009476C, D_8011FB1C);
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_80120630;
    D_8012067C = D_80120650;
    D_80120680 = D_80120670;
    func_80078970(D_80094784, D_8011FB28);
    func_80078970(D_800947C4, D_8011FB3C);
    func_80011DFC();
}
