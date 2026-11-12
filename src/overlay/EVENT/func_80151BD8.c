/* Address: 0x80151BD8 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800433D0();
extern void FUN_80110094();

void func_80151BD8(void)
{
    FUN_80015D28(134, 0x801B0000, 44044);
    FUN_800433D0(512);
    FUN_80110094();
    FUN_80011DFC();
}
