/* Address: 0x8009FCB4 */
/* Size: 96 bytes (24 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers t9 t6 */
#include "types.h"

typedef void (*SndFunc)(u8 *, s32, s32, s32);

extern u8 D_800EBD62;
extern u8 graph_env;
extern u8 libgpu_graph_reverse_flag;
extern SndFunc GPU_printf;
extern u8 libgpu_setgraphdebug_trace_fmt;

s32 SetGraphDebug(s32 a0) {
    s32 old;
    s32 note;

    old = D_800EBD62;
    D_800EBD62 = a0;
    note = a0 & 0xFF;
    if (note != 0) {
        GPU_printf(&libgpu_setgraphdebug_trace_fmt, note, graph_env, libgpu_graph_reverse_flag);
    }
    return old;
}
