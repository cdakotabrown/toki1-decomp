/* Address: 0x80157DC8 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_801101B4();

void func_80157DC8(void)
{
    FUN_80015D28(34, 0x801E0000, 44010);
    FUN_801101B4();
    FUN_80011DFC();
}
