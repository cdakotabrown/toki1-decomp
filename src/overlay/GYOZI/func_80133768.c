/* @SymbolStart: 0x80133730 */
/* @SymbolSize: 96 */
#include "types.h"

extern void func_8004DDD8(void);

extern u8 D_80145F60;
extern s16 D_80145EB4;
extern s32 D_80145EA8;
extern s32 D_80145EAC;
extern s32 D_80145EB0;
extern s32 D_80145E00;
extern s32 D_80145E38;
extern s32 D_80145E70;

void func_80133768(void)
{
    D_80145F60 = 1;
    D_80145EB4 = 0;
    D_80145EA8 = D_80145E00;
    D_80145EAC = D_80145E38;
    D_80145EB0 = D_80145E70;
    func_8004DDD8();
}
