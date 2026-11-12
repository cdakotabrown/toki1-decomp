/* Address: 0x80041744 */
/* Size: 48 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void sprite_init(s32 arg0, s32 arg1);
extern void bg_init(s32 arg0, s32 arg1);

void prim_init(void) {
    sprite_init(0, 64);
    bg_init(0, 2);
}
