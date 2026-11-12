/* Address: 0x8009E860 */
/* Size: 200 bytes (50 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x4 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

extern MATRIX GsIDMATRIX;

void Gssub_make_matrix(MATRIX *m, s16 s, s16 c, s8 axis) {
    *m = GsIDMATRIX;
    switch (axis) {
    case 'X':
    case 'x':
        m->m[1][1] = c;
        m->m[1][2] = -s;
        m->m[2][2] = c;
        m->m[2][1] = s;
        break;
    case 'Y':
    case 'y':
        m->m[0][0] = c;
        m->m[2][2] = c;
        m->m[0][2] = s;
        m->m[2][0] = -s;
        break;
    case 'Z':
    case 'z':
        m->m[0][0] = c;
        m->m[1][1] = c;
        m->m[0][1] = -s;
        m->m[1][0] = s;
        break;
    }
}
