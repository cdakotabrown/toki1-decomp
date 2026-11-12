/* Address: 0x80147614 */
/* Size: 100 bytes (25 instructions) */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=4 */
#include "types.h"

extern void func_8004BC20(s32 idx);
extern void func_80011DFC(void);

extern s8 D_800B1746;
extern u8 D_800B0860[];

void func_80147614(void)
{
    u8 *rec;

    D_800B1746 = 4;
    func_8004BC20(4);

    rec = &D_800B0860[(u8)D_800B1746 * 52];
    rec[432] = (u8)(rec[432] & 0xFFFD);

    func_80011DFC();
}
