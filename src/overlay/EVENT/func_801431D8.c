/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x158 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x15c */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x160 */
/* Address: 0x801431D8 */
/* Size: 380 bytes (95 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern u8 D_800F19AB;
extern u8 D_800B0E42;
extern u8 D_800B0E43;
extern s16 D_80094714;
extern s16 D_80094718;
extern s16 D_800B0AD6;
extern s16 D_800B0ADA;
extern s16 D_800B094E;
extern s32 D_801203FC;
extern s32 D_80120414;
extern s32 D_8012042C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern u8 D_8011F9CC[];
extern u8 D_80094784[];
extern u8 D_8011F9D0[];

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

void func_801431D8(void) {
    D_800B1746 = 4;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011F9CC);
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
    D_80120678 = D_801203FC;
    D_8012067C = D_80120414;
    D_80120680 = D_8012042C;
    func_80078970(D_80094784, D_8011F9D0);
    if (D_800B0E43 == 0) {
        if (D_800B0E42 == 0) {
            D_800B0AD6 = (s16)(D_800B0AD6 + 1);
        }
    }
    D_800B0ADA = (s16)(D_800B0ADA + 1);
    D_800B094E = (s16)(D_800B094E + 10);
    func_8004C250();
    func_80011DFC();
}
