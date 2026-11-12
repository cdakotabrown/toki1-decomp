/* Address: 0x800A03B4 */
/* Size: 104 bytes (26 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_registers t9:t7 --only 0x20,0x24,0x34 */
/* @PostProc: swap_registers t7:t8 --only 0x3c,0x40,0x48 */
#include "types.h"

extern u8 D_800EBD62;
extern void (*GPU_printf)(s32, s32, s32);
extern s32 libgpu_clearotagr_trace_fmt;

typedef struct {
    s32 pad[11];
    s32 (*func_2C)();
} SomeStruct;

extern SomeStruct *graph_jtb;

s32 func_800A03BC(s32 arg0, s32 arg1) {
    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf((s32)&libgpu_clearotagr_trace_fmt, arg0, arg1);
    }
    graph_jtb->func_2C(arg0, arg1);
    return arg0;
}
