/* Address: 0x80139974 (absorbed head)  Size: 296 bytes (74 instructions) */
/* @SymbolStart: 0x80139974 */
/* @SymbolSize: 296 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=12 */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800B0F30;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern u8 D_800F19AB;
extern u16 D_80094714;
extern u16 D_80094718;
extern s32 D_8012061C;
extern s32 D_8012063C;
extern s32 D_8012065C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_8011F710;
extern s32 D_8011F714;
extern s32 D_80094784;

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
extern void func_8004BC20();
extern void func_8004CDDC();
extern void func_80078970();
extern void func_80011DFC();

void func_8013997C(void) {
    D_800B1746 = 12;
    D_800B0F30 = 12;

    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(&D_8011F710);

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

    D_80120678 = D_8012061C;
    D_8012067C = D_8012063C;
    D_80120680 = D_8012065C;

    func_80078970(&D_80094784, &D_8011F714);
    func_80011DFC();
}
