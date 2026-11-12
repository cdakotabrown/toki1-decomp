/* @SymbolStart: 0x801575A8 */
/* @SymbolSize: 100 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=128 */
#include "types.h"

extern void func_80011DFC(void);

extern s32 ev_flag04;
extern u8 D_800EC1A6;
extern u8 D_800EC1A5;
extern u8 D_800EC1A4;
extern u8 D_800EC1CA;
extern u8 D_800EC1C9;
extern u8 D_800EC1C8;
extern u8 D_800E9E63;

void func_801575E4(void)
{
    ev_flag04 = 1;
    D_800EC1A6 = 0x80;
    D_800EC1A5 = 0x80;
    D_800EC1A4 = 0x80;
    D_800EC1CA = 0x80;
    D_800EC1C9 = 0x80;
    D_800EC1C8 = 0x80;
    D_800E9E63 = 0x80;
    func_80011DFC();
}
