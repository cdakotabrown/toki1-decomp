/* @PostProc: patch_insn_reg_field 0x140:rd:t8:t9 0x144:rt:t8:t9 0x15c:rd:t5:t6 0x160:rs:t5:t6 0x168:rt:t6:t7 0x170:rs:t6:t7 0x170:rt:t7:t8 0x174:rt:t7:t8 */
/* Address: 0x8014691C */
/* Size: 400 bytes (100 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern s16 D_80094714;
extern s16 D_80094718;
extern u8 D_800F19AB;
extern s32 D_80120628;
extern s32 D_80120648;
extern s32 D_80120668;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_800EECB0;
extern s16 D_800B096E;
extern s16 D_800B0860[][26];
extern u8 D_8011FB10[];
extern u8 D_80094748[];
extern u8 D_800B0929[];

extern void func_80012D2C();
extern void func_8001886C();
extern void func_80010678();
extern void func_80018944();
extern void func_80012D40();
extern void func_80017E50();
extern void func_8001FD50();
extern void func_80036884();
extern void func_80045B14();
extern void func_800F6000();
extern void func_8003535C();
extern void func_8003580C();
extern void func_8004C360();
extern void func_80078970();
extern void func_8004CDDC();
extern void func_8004C250();
extern void func_80011DFC();

void func_8014691C(void) {
    s16 i;
    s16 *p;

    D_800B1746 = 11;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011FB10);
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
    func_80078970(D_80094748, D_800B0929);
    func_8004CDDC();
    D_80120678 = D_80120628;
    D_8012067C = D_80120648;
    D_80120680 = D_80120668;
    if (D_800EECB0 == 1) {
        D_800B096E -= 5;
        for (i = 0; i < 10; i++) {
            p = &D_800B0860[i][0];
            if ((*(s32 *)(p + 216) << 30) < 0) {
                p[211] -= 1;
            }
        }
        func_8004C250(D_800B0860);
    }
    func_80011DFC();
}
