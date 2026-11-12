/* Address: 0x801567FC */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_8010BB90();

void func_801567FC(void)
{
    FUN_80015D28(2, 0x80197000, 41184);
    FUN_8010BB90();
    FUN_80011DFC();
}
