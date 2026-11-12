/* Address: 0x8009FD14 (absorbed head; symbol name keeps the prologue address) */
/* Size: 156 bytes (39 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8009FD14 */
/* @SymbolSize: 156 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 32 --new 40 --shift-ra */
/* @PostProc: remap_stack_slots --map 20:28,28:36,32:40 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t9:t8 0x2c:rs:t9:t8 0x2c:rt:t9:t8 0x38:rs:t9:t8 0x48:rt:t8:t9 0x50:rs:t8:t9 0x64:rt:t9:t2 0x6c:rs:t9:t2 0x74:rt:t2:t3 0x88:rt:t2:t3 */
#include "types.h"

extern volatile u8 libgpu_graph_queue_mode;
extern u8 D_800EBD62;
extern void (*GPU_printf)(u8 *, s32);
extern u8 libgpu_setgraphqueue_trace_fmt[];
extern void **graph_jtb;
extern void DMACallback(s32, s32);

s32 SetGraphQueue(s32 arg0) {
    s32 orig;
    u32 n;

    n = D_800EBD62;
    orig = libgpu_graph_queue_mode;

    if (n >= 2u) {
        GPU_printf(libgpu_setgraphqueue_trace_fmt, arg0);
    }
    if (arg0 != libgpu_graph_queue_mode) {
        ((void (*)(s32))graph_jtb[13])(1);
        libgpu_graph_queue_mode = arg0;
        DMACallback(2, 0);
    }
    return orig;
}
