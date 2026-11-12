/* Address: 0x8009FEA8 */
/* Size: 96 bytes (24 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_registers t9:t7 --only 0x1c,0x20,0x2c */
/* @PostProc: swap_registers t7:t8 --only 0x34,0x38,0x40 */
#include "types.h"

extern u8 D_800EBD62;
extern void (*GPU_printf)(s32, s32);
extern s32 libgpu_drawsync_trace_fmt;

typedef struct {
    s32 pad[15];
    s32 (*func_3C)();
} Struct_8009FEA8;

extern Struct_8009FEA8 *graph_jtb;

void DrawSync(s32 arg0) {
    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf((s32)&libgpu_drawsync_trace_fmt, arg0);
    }
    graph_jtb->func_3C(arg0);
}
