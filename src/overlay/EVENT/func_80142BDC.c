/* Address: 0x80142BDC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_801053B0();

void func_80142BDC(void)
{
    FUN_80015D28(61, 0x801B0000, 34796);
    FUN_801053B0();
    FUN_80011DFC();
}
