/* Address: 0x800A0480 */
/* Size: 96 bytes (24 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_registers_auto --swaps t7:v0 */
/* @PostProc: swap_registers_auto --swaps t9:t7 */
/* @PostProc: swap_registers_liveness --target-exe disks/SLPS_000.65 --address 0x800A0480 */
#include "types.h"

extern u8 D_800EBD62;
extern void (*GPU_printf)(s32, s32);
extern s32 libgpu_drawotag_trace_fmt;

typedef struct {
    s32 pad_00;
    s32 (*func_04)();
    s32 pad_08[4];
    s32 field_18;
} Struct_800A0488;

extern Struct_800A0488 *graph_jtb;

void func_800A0488(s32 arg0) {
    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf((s32)&libgpu_drawotag_trace_fmt, arg0);
    }
    graph_jtb->func_04(graph_jtb->field_18, arg0, 0);
}
