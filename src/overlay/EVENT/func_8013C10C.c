/* Address: 0x8013C10C */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800FFBE0();

void func_8013C10C(void)
{
    FUN_80015D28(61, 0x801B0000, 32928);
    FUN_800FFBE0();
    FUN_80011DFC();
}
