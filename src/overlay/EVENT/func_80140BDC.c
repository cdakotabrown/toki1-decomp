/* Address: 0x80140BDC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_80103DE0();

void func_80140BDC(void)
{
    FUN_80015D28(69, 0x801B0000, 34337);
    FUN_80103DE0();
    FUN_80011DFC();
}
