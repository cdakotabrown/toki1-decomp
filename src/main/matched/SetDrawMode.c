/* Address: 0x800A0D80 */
/* Size: 120 bytes (30 instructions) */
/* Region: Library (0x80086000+), IDO 5.2, stock pic0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: strip_trailing_instructions --count 2 --nop-only */
/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=all IDO52_FORCE_WEB_COLOR_NOSPLIT=6,8 IDO52_FORCE_WEB_COLOR=0:v1,6:t0,8:t1 IDO52_FORCE_WEB_NOSPILL=0.0:5 */
#include "types.h"

extern s32 get_mode(s32 arg0, s32 arg1, u32 arg2);
extern s32 get_tw(s32 arg);

void SetDrawMode(volatile s32 *ptr, s32 a1, s32 a2, u32 a3, s32 arg4) {
    *ptr = (*ptr & 0x00FFFFFF) | 0x02000000;
    ptr[1] = get_mode(a1, a2, (u16)a3);
    ptr[2] = get_tw(arg4);
}
