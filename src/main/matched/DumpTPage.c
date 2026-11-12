/* Address: 0x800A2A8C */
/* Size: 208 bytes (52 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2, native as1 */
/* @CompileFlags: -Wc,-pic0 -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=6:v1 IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_UGEN_DEST_TEMP=13,26 IDO52_AS1_KEEP_ANDI=0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void (*GPU_printf)(void *, u32, u32, u32, u32);
extern u8 libgpu_dumptpage_type12_fmt;
extern u8 libgpu_dumptpage_other_fmt;

extern int GetGraphType();

void DumpTPage(v)
u16 v;
{
    if (GetGraphType() == 1 || GetGraphType() == 2) {
        GPU_printf(&libgpu_dumptpage_type12_fmt, (v >> 9) & 3, (v >> 7) & 3,
                   (v << 6) & 0x7C0, (v << 3) & 0x300);
    } else {
        GPU_printf(&libgpu_dumptpage_other_fmt, (v >> 7) & 3, (v >> 5) & 3,
                   (v << 6) & 0x7C0,
                   ((v << 4) & 0x100) + ((v >> 2) & 0x200));
    }
}
