/* Address: 0x8013A138 */
/* Size: 312 bytes (78 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern s16 D_80094714;
extern s16 D_80094718;
extern u8 D_80094784;
extern u8 D_8011F738;
extern u8 D_8011F734;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern u8 D_800F19AB;
extern s32 D_801203B4;
extern s32 D_801203CC;
extern s32 D_801203E4;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s16 D_800B0AA6;

extern void func_80012D2C(s32, s32);
extern void func_8001886C(s32);
extern void func_80010678(void);
extern void func_80018944(s32);
extern void func_80012D40(s32);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_80036884(s32);
extern void func_80045B14(void);
extern void func_800F6000(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_8004BC20(u8);
extern void func_8004CDDC(void);
extern void func_80078970(s32, s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013A138(void) {
    D_800B1746 = 3;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884((s32)&D_8011F734);
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
    D_80120678 = D_801203B4;
    D_8012067C = D_801203CC;
    D_80120680 = D_801203E4;
    func_80078970((s32)&D_80094784, (s32)&D_8011F738);
    D_800B0AA6 = D_800B0AA6 + 2;
    func_8004C250();
    func_80011DFC();
}
