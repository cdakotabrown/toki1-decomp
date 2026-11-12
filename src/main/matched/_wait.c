/* Address: 0x8008BB08, Size: 28 bytes (7 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* pipeline (`@Compiler: ido52-gas`) the loop assembles to retail's seven */
/* @Compiler: ido52-gas */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void _wait(void) {
    s32 i;

    for (i = 0; i != 2000; i += 4) {
    }
}
