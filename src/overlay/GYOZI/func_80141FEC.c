/* @SymbolStart: 0x80141fe4 */
/* @SymbolSize: 52 */
#include "types.h"

extern void func_8004DE1C(void);

extern s16 D_80148EAC;
extern s16 D_8012CAD4;

void func_80141FEC(void)
{
    D_80148EAC = 1;
    D_8012CAD4 = -100;
    func_8004DE1C();
}
