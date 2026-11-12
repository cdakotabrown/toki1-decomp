/* @SymbolStart: 0x8013b060 */
/* @SymbolSize: 72 */
#include "types.h"

extern void func_800BCE10(void *a, void *b);
extern void func_8004DE1C(void);

extern s16 D_801474A8;
extern s16 D_801474AC;
extern char D_800D92A0[];
extern char D_80145BEC[];

void func_8013B068(void)
{
    D_801474A8 = 5;
    D_801474AC = 0;
    func_800BCE10(D_800D92A0, D_80145BEC);
    func_8004DE1C();
}
