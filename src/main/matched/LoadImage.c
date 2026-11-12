/* Address: 0x800A00E0 */
/* Size: 80 bytes (20 instructions) */
/* Region: IDO (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_registers_auto --swaps t6:v0 */
/* @PostProc: swap_registers_auto --swaps t9:t6 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 (*func_ptr)();
    s32 padding[5];
    s32 field_20;
} SomeStruct_800A00E0;

extern SomeStruct_800A00E0 *graph_jtb;
extern s32 libgpu_loadimage_name_str;

extern void checkRECT(s32 *ptr, s32 val);

void LoadImage(s32 a0, s32 a1) {
    checkRECT(&libgpu_loadimage_name_str, a0);
    graph_jtb->func_ptr(graph_jtb->field_20, a0, 8, a1);
}
