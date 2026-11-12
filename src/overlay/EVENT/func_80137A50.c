/* Address: 0x80137A50 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800F9BDC();

void func_80137A50(void)
{
    FUN_80015D28(53, 0x801B0000, 31936);
    FUN_800F9BDC();
    FUN_80011DFC();
}
