/* @SymbolStart: 0x80133628 */
/* @SymbolSize: 104 */
#include "types.h"

extern void func_8004DE1C(void);

extern s16 D_80145EB4;
extern s32 D_80145E08;
extern s32 D_80145E40;
extern s32 D_80145E78;
extern s32 D_80145EA8;
extern s32 D_80145EAC;
extern s32 D_80145EB0;
extern u8 D_800F647A;

void func_80133658(void)
{
    D_80145EB4 = 7;
    D_80145EA8 = D_80145E08;
    D_80145EAC = D_80145E40;
    D_80145EB0 = D_80145E78;
    func_8004DE1C();
    D_800F647A -= 5;
}
