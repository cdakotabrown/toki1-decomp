/* func_801343BC -- GYOZI overlay, Address: 0x801343BC, Size: 152 bytes (38 insns), Semi-matched  * Status: BYTE-PERFECT 🎉 */
#include "types.h"

extern void func_80136220(void);
extern void func_8004DE1C(void);

extern u8 D_800F647A;
extern s32 *D_80146150;
extern s32 *D_80146158;
extern s32 *D_80146160;

void func_801343BC(void)
{
    func_80136220();
    if ((*D_80146150 & 0x80000000) &&
        (*D_80146158 & 0x80000000) &&
        (*D_80146160 & 0x80000000)) {
        func_8004DE1C();
    } else {
        D_800F647A -= 2;
    }
}
