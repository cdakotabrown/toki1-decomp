/* Address: 0x8013CECC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800FFD40();

void func_8013CECC(void)
{
    FUN_80015D28(61, 0x801B0000, 33050);
    FUN_800FFD40();
    FUN_80011DFC();
}
