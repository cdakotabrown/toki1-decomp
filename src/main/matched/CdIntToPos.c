/* Address: 0x800880CC */
/* Size: 264 bytes (66 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC (magic-number division, not IDO div) */
/* @Compiler: gcc263p */
/* @CompileFlags: -O2 -fsched2-forward */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

#define itob(i) (((i) / 10) * 16 + (i) % 10)

typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;

CdlLOC *CdIntToPos(s32 i, CdlLOC *p) {
    i += 150;
    p->sector = itob(i % 75);
    i /= 75;
    p->second = itob(i % 60);
    i /= 60;
    p->minute = itob(i);
    return p;
}
