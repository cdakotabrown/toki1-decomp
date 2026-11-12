/* Address: 0x800467C0 */
/* Size: 128 bytes (32 instructions) */
/* Compiler: GCC-like (game code region) */
/* Region: 0x80041000-0x80080FFF */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 CdControlB(s32 cmd, u8 *buf, s32 size);
extern void VSync(s32 arg);

extern u8 cd_mode;

void cd_mode_set(void) {
    u8 *buf;
    s32 i;

    buf = &cd_mode;
    cd_mode = 0x84;

    if (CdControlB(14, buf, 0) == 0) {
        do {
        } while (CdControlB(14, buf, 0) == 0);
    }

    for (i = 0; i < 512; i++) {
        VSync(0);
    }
}
