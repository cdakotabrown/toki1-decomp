/* Address: 0x8009D120 */
/* Size: 76 bytes (19 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 -O1 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_registers v0:t9 */
/* @PostProc: swap_registers v1:v0 */
/* @PostProc: swap_registers a1:v1 */
/* @PostProc: swap_registers a2:a1 */
/* @PostProc: swap_registers t9:a2 */
#include "types.h"

typedef struct {
    s32 m[8];
} LCMATRIX;

extern LCMATRIX _LC;

void gte_read_lc(LCMATRIX *m) {
    s32 a, b, c;

    a = _LC.m[0];
    b = _LC.m[1];
    c = _LC.m[2];
    m->m[0] = a;
    m->m[1] = b;
    m->m[2] = c;
    c = _LC.m[5];
    b = _LC.m[4];
    a = _LC.m[3];
    m->m[5] = c;
    m->m[4] = b;
    m->m[3] = a;
    b = _LC.m[7];
    a = _LC.m[6];
    m->m[7] = b;
    m->m[6] = a;
}
