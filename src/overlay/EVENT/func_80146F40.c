/* Address: 0x80146F40 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_8010A280();

void func_80146F40(void)
{
    FUN_80015D28(61, 0x801B0000, 35800);
    FUN_8010A280();
    FUN_80011DFC();
}
