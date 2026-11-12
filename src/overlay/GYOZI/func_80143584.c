/* @SymbolStart: 0x80143578 */
/* @SymbolSize: 68 */
#include "types.h"

extern void func_80086AB0(s32);
extern void func_8004DE1C(void);

extern s16 D_8012B960;
extern s16 D_8012B950;
extern s8 D_8012B94A;

void func_80143584(void)
{
    D_8012B960 = 0;
    D_8012B950 = 0;
    D_8012B94A = 3;
    func_80086AB0(1283);
    func_8004DE1C();
}
