/* Function: SetDispMask */
/* Address: 0x8009FE28 */
/* Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: custom_script postproc_SetControlChange.py */
#include "types.h"

typedef void (*FuncPtr)(void*, s32);

extern u8 D_800EBD62;
extern FuncPtr GPU_printf;
extern u8 libgpu_setdispmask_trace_fmt;

typedef struct {
    u32 pad[4];
    void (*func16)(u32);
} SStruct;

extern SStruct *graph_jtb;

void SetDispMask(s32 arg0) {
    u32 param;

    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf(&libgpu_setdispmask_trace_fmt, arg0);
    }

    if (arg0 != 0) {
        param = 0x03000000;
    } else {
        param = 0x03000001;
    }

    graph_jtb->func16(param);
}
