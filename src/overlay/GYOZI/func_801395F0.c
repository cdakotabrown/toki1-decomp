/* @SymbolStart: 0x801395e8 */
/* @SymbolSize: 92 */
#include "types.h"

extern void func_800BCE10(void *a, void *b);
extern void func_8004DE1C(void);

extern s16 D_801474A8;
extern s16 D_801474AC;
extern char D_800D92A0[];
extern char D_800D92E0[];
extern char D_80145B30[];
extern char D_80145B38[];

void func_801395F0(void)
{
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(D_800D92A0, D_80145B30);
    func_800BCE10(D_800D92E0, D_80145B38);
    func_8004DE1C();
}
