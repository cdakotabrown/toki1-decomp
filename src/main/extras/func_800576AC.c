/* Address: 0x800576AC */
/* Size: 108 bytes (27 instructions) */
/* Compiler: GCC-like (0x80040000-0x80080000 region) */
#include "types.h"

extern s32 CdControl(s32, s32, s32);
extern s32 CdRead2(s32);

void func_800576AC(s32 arg0) {

    while (CdControl(21, arg0, 0) == 0) {

    }

    while (CdRead2(448) == 0) {

    }
}
