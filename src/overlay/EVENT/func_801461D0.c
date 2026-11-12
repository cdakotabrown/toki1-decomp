/* func_801461D0 -- EVENT overlay, Address: 0x801461D0, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_80011DFC(void);
extern void func_80015D28(s32, s32, s32);
extern void func_80108280(void);

void func_801461D0(void)
{
    func_80015D28(69, 0x801B0000, 35609);
    func_80108280();
    func_80011DFC();
}
