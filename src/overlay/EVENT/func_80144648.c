/* Address: 0x80144648 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_80107FC0();

void func_80144648(void)
{
    FUN_80015D28(69, 0x801B0000, 35357);
    FUN_80107FC0();
    FUN_80011DFC();
}
