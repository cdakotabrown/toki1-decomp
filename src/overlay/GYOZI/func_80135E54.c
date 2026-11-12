/* @SymbolStart: 0x80135e24 */
/* @SymbolSize: 96 */
#include "types.h"

extern void func_80137E84(void);
extern void func_8004DE1C(void);

extern s16 D_800D9248;
extern s32 D_80146358;
extern s32 D_8014635C;
extern s32 D_80146360;
extern s32 D_800D9258;
extern s32 D_800D925C;
extern s32 D_800D9260;

void func_80135E54(void)
{
    D_800D9248 = 29;
    D_800D9258 = D_80146358;
    D_800D925C = D_8014635C;
    D_800D9260 = D_80146360;
    func_80137E84();
    func_8004DE1C();
}
