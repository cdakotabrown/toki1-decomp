/* Address: 0x8009D370 */
/* Size: 24 bytes (6 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad[28];
} Elem28;

typedef struct {
    s32 pad0;
    s32 pad4;
    Elem28 *cur;
} Cursor;

void GsLinkObject2(Elem28 *base, Cursor *c, s32 n) {
    c->cur = base + n;
}
