/* Address: 0x800A0210 */
/* Size: 420 bytes (105 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_PASS1_PROP=all IDO52_UGEN_DEST_TEMP=3 IDO52_AS1_TIE_TAKE=27 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_COPY_PROP=3 IDO52_CONST_DEF_BB=55:6:- IDO52_RELOAD_SUPPRESS=55:1 IDO52_CONST_DEF_EDGE=55:2:j,55:9:j */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 D_800EBD62;
extern void (*GPU_printf)();
extern char libgpu_clearotag_trace_fmt[];

u32 *ClearOTag(u32 *ot, s32 n) {
    if (D_800EBD62 >= 2U) {
        GPU_printf(libgpu_clearotag_trace_fmt, ot, n);
    }
    while (--n) {
        *ot &= 0x00FFFFFF;
        { u32 v = *(volatile u32 *)ot; *ot = ((((u32)(ot + 1) ^ v) << 8) >> 8) ^ v; }
        ot++;
    }
    *ot = 0x00FFFFFF;
    return ot;
}
