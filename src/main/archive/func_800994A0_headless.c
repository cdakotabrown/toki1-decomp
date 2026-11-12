/* Function: func_800994A0 */
/* Address: 0x800994A0 */
/* Size: 120 bytes (30 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --fill-branch-store */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x4 */
#include "types.h"

extern u16 PSDBASEX;
extern u16 libgs_dispbuff_x1;
extern u16 PSDBASEY;
extern u16 libgs_dispbuff_y1;
extern u16 libgs_psdofsy_buf1;

extern void GsSetDrawBuffClip(void);
extern void GsSetDrawBuffOffset(void);

void func_800994A0(s32 a0, s32 a1, s32 a2, s32 a3) {
    register s32 v0_arg __asm__("$2");
    libgs_psdofsy_buf1 = a3;
    if (v0_arg != 0) {
        PSDBASEX = 0;
        libgs_dispbuff_x1 = 0;
        PSDBASEY = 0;
        libgs_dispbuff_y1 = 0;
    } else {
        PSDBASEX = a0;
        libgs_dispbuff_x1 = a2;
        PSDBASEY = a1;
        libgs_dispbuff_y1 = a3;
    }
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}
