/* Function: GsInit3D  (GsInit3D) */
/* Address: 0x80099520  (registered address corrected 2026-09-17; see below) */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (0x80086000+) -- and this one is **GCC**, not IDO. */
/* @SymbolStart: 0x80099520 */
/* @SymbolSize: 112 */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileFlags: -O2 -G0 -mips1 -O2 -G0 -mips1 --load-base-nop --konami-lib-sched */
#include "types.h"

extern s16 scr_half_w;
extern s16 scr_half_h;
extern s32 VWD0;
extern s32 HWD0;
extern s32 GsLIGHT_MODE;
extern s32 GsCLIP3far;
extern s32 GsCLIP3near;
extern void GsSetDrawBuffOffset(void);

void GsInit3D(void) {
    scr_half_w = (s16)(HWD0 / 2);
    scr_half_h = (s16)(VWD0 / 2);
    GsSetDrawBuffOffset();
    GsCLIP3near = 10;
    GsLIGHT_MODE = 0;
    GsCLIP3far = 16383;
}
