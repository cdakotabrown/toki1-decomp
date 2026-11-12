/* @SymbolStart: 0x80138dbc */
/* @SymbolSize: 72 */
#include "types.h"

extern void func_800BCE10(void *a, void *b);
extern void func_8004DE1C(void);

extern s16 D_801474A8;
extern s16 D_801474AC;
extern char D_800D92A0[];
extern char D_80145B10[];

void func_80138DC4(void)
{
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(D_800D92A0, D_80145B10);
    func_8004DE1C();
}
