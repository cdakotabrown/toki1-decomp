/* Function: SetGraphReverse */
/* Address: 0x8009FBAC */
/* Size: 264 bytes (66 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8009FBAC */
/* @SymbolSize: 264 */
/* @Compiler: ido52 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_FORCE_WEB_COLOR=41:v1 IDO52_UGEN_TEMP_REG=3:t9,4:t0,5:t2,6:t4,7:t5,8:t6 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 32 --new 40 */
/* @PostProc: remap_stack_slots --keep-frame --map '20:28,24:32,28:36,32:40' */
/* @PostProc: patch_insn_reg_field --patch 0x28:rt:t9:t8 --patch 0x2c:rs:t9:t8 --patch 0x2c:rt:t9:t8 --patch 0x38:rs:t9:t8 --patch 0x70:rt:t9:t1 --patch 0x78:rs:t9:t1 --patch 0x8c:rt:t9:t3 --patch 0x94:rd:v1:a0 --patch 0x98:rs:t9:t3 --patch 0x9c:rs:v1:a0 --patch 0xe8:rt:t9:t7 --patch 0xf0:rs:t9:t7 */
#include "types.h"

extern u8 graph_env;
extern u8 D_800EBD62;
extern u8 libgpu_graph_reverse_flag;
extern void (*GPU_printf)(u8 *);
extern u8 libgpu_setgraphreverse_trace_fmt[];

typedef struct {
    s32 pad0[4];
    s32 (*func_10)(s32);
    s32 pad1[5];
    s32 (*func_28)(s32);
} AudioStruct;

extern AudioStruct *graph_jtb;

s32 SetGraphReverse(s32 arg0) {
    s32 orig;
    s32 flag;
    s32 st;
    s32 cmd;

    orig = libgpu_graph_reverse_flag;
    if ((u32)D_800EBD62 >= 2U) {
        GPU_printf(libgpu_setgraphreverse_trace_fmt);
    }
    libgpu_graph_reverse_flag = arg0;
    if ((arg0 & 0xFF) != 0) {
        flag = 128;
    } else {
        flag = 0;
    }
    st = graph_jtb->func_28(8);
    flag = flag | st;
    graph_jtb->func_10(flag | 0x08000000);
    if (graph_env == 2) {
        if (libgpu_graph_reverse_flag != 0) {
            cmd = 0x20000501;
        } else {
            cmd = 0x20000504;
        }
        graph_jtb->func_10(cmd);
    }
    return orig;
}
