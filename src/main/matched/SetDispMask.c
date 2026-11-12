/* Address: 0x8009FE28   Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8009FE28 */
/* @SymbolSize: 128 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=1 IDO52_UGEN_TEMP_REG=2:t8,3:t9 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: patch_insn_reg_field 0x1c:rt:t9:t7 0x20:rs:t9:t7 0x20:rt:t9:t7 0x2c:rs:t9:t7 0x60:rt:t9:t0 0x68:rs:t9:t0 */
#include "types.h"

extern u8 D_800EBD62;
extern void (*GPU_printf)(u8 *, s32);
extern u8 libgpu_setdispmask_trace_fmt;

typedef struct {
    s32 pad[4];
    void (*func_10)(s32);
} Struct_FE30;

extern Struct_FE30 *graph_jtb;

void SetDispMask(s32 arg0) {
    s32 cmd;

    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf(&libgpu_setdispmask_trace_fmt, arg0);
    }
    if (arg0 != 0) {
        cmd = 0x03000000;
    } else {
        cmd = 0x03000001;
    }
    graph_jtb->func_10(cmd);
}
