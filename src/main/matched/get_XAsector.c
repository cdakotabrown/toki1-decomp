/* Address: 0x80046554, Size: 12 bytes (3 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 XAsector[];

void *get_XAsector(void) {
    return (void *)XAsector;
}
