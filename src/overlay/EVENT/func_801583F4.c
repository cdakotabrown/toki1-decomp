/* Address: 0x801583F4 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_801252E4;
extern u8 D_801243B0;

extern s32 func_8002328C(s32 val);
extern void func_80011EC4(s32 arg);
extern void func_80011DFC(void);

void func_801583F4(void)
{
    s32 v0;

    v0 = func_8002328C(D_800B1746);
    if (D_800B1746 != 7) goto L_FAIL;
    if ((u32)v0 < 3U) goto L_FAIL;
    if (D_801252E4 != 2) goto L_FAIL;
    func_80011EC4(57);
    D_801243B0 = D_801243B0 + 1;
    goto L_DONE;
L_FAIL:
    func_80011DFC();
L_DONE:
    ;
}
