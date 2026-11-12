/* Function: DumpTPage */
/* Address: 0x800A2A8C */
/* Size: 208 bytes (52 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_func_800A2A8C.py */
#include "types.h"

extern s32 GetGraphType(void);
extern void (*GPU_printf)(u8 *, s32, s32, s32, s32);
extern u8 libgpu_dumptpage_type12_fmt;
extern u8 libgpu_dumptpage_other_fmt;

void DumpTPage(u32 arg0) {
    s32 result;
    u16 cmd;
    u32 c;

    cmd = (u16)arg0;
    c = cmd;
    result = GetGraphType();
    if (result == 1) goto mode1;
    result = GetGraphType();
    if (result != 2) goto mode2;

mode1:
    (*GPU_printf)(&libgpu_dumptpage_type12_fmt,
               (s32)((c >> 9) & 3u),
               (s32)((c >> 7) & 3u),
               (s32)((c << 6) & 0x7C0u),
               (s32)((c << 3) & 0x300u));
    goto done;

mode2:
    (*GPU_printf)(&libgpu_dumptpage_other_fmt,
               (s32)((c >> 7) & 3u),
               (s32)((c >> 5) & 3u),
               (s32)((c << 6) & 0x7C0u),
               (s32)(((c << 4) & 0x100u) + ((c >> 2) & 0x200u)));
done:
    return;
}
