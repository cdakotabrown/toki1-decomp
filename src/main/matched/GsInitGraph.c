/* Function: GsInitGraph */
/* Address: 0x80098DD0 */
/* Size: 116 bytes (29 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void gpu_init(s32, s32, s32, s32, s32);
extern void gte_init(void);
extern void valiable_init(s32, s32);
extern void GsSetDrawBuffClip(void);
extern void GsSetDrawBuffOffset(void);

extern s16 PSDIDX;

void GsInitGraph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    s32 x = arg1 & 0xFFFF;
    s32 y = arg0 & 0xFFFF;

    gpu_init(y, x, arg2 & 0xFFFF, arg3 & 0xFFFF, arg4);
    gte_init();
    PSDIDX = 0;
    valiable_init(y, x);
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}
