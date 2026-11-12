/* Address: 0x801428D0 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_801051A0();

void func_801428D0(void)
{
    FUN_80015D28(61, 0x801B0000, 34605);
    FUN_801051A0();
    FUN_80011DFC();
}
