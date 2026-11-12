/* Function: DrawSyncCallback */
/* Address: 0x8009FDD0 */
/* Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: patch_insn_reg_field 0x1c:rt:t9:t7 0x20:rs:t9:t7 0x20:rt:t9:t7 0x2c:rs:t9:t7 0x3c:rt:t7:t8 0x54:rt:t7:t8 */
/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=all */
#include "types.h"

typedef void (*FuncPtr)(void*, s32);

extern u8 D_800EBD62;
extern s32 libgpu_drawsync_callback;
extern FuncPtr GPU_printf;
extern u8 libgpu_drawsynccb_trace_fmt;

s32 DrawSyncCallback(s32 arg0) {
    s32 old;
    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf(&libgpu_drawsynccb_trace_fmt, arg0);
    }
    old = libgpu_drawsync_callback;
    libgpu_drawsync_callback = arg0;
    return old;
}
