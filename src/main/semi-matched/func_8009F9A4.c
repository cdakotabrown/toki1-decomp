/* @SymbolStart: 0x8009F99C */
/* @SymbolSize: 528 */
/* Function: func_8009F9A4 */
/* Address: 0x8009F9A4 */
/* Size: 520 bytes (130 instructions) */
/* Region: Library (0x80086000+) */
/* Status: Semi-matched - CONFIRMED UNMATCHABLE FROM C */
/* @PostProc: convert_or_to_addu */
/* @Measured: 2026-09-24 3/132 exact (2.8%), compiled 107 vs target 132 words, Fixup 0 -- SEMI */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=5:t7 */
#include "types.h"

typedef void (*SndFunc)(s32, s32);
typedef s32 (*SndFuncRet)(s32);

extern SndFunc GPU_printf;
extern u8 libgpu_resetgraph_jtb_fmt[];
extern u8 libgpu_default_jtb_tbl[];
extern u8 libgpu_resetgraph_trace_fmt[];
extern u8 graph_env;
extern u8 libgpu_graph_queue_mode;
extern u16 D_800EBD64;
extern u16 D_800EBD66;
extern u8 libgpu_cur_drawenv[92];
extern u8 libgpu_graph_disp_env_cache[20];

typedef struct {
    u8 pad0[16];
    void (*func_10)(s32);
    u8 pad1[32];
    SndFuncRet func_34;
} SndContext;

extern SndContext *graph_jtb;

extern void printf(u8 *, u8 *, u8 *);
extern void memset(void *, s32, s32);
extern void ResetCallback(void);
extern void set_jtb(SndContext *);

void func_8009F9A4(s32 arg0) {
    s32 result;
    s32 mode;

    mode = arg0 & 3;

    if (mode == 0) {

        printf(libgpu_resetgraph_jtb_fmt, libgpu_default_jtb_tbl, &graph_env);
        memset(&graph_env, 0, 128);
        libgpu_graph_queue_mode = 1;
        ResetCallback();
        set_jtb(graph_jtb);
        result = graph_jtb->func_34(0);
        graph_env = (u8)result;

        mode = result & 0xff;
    }

    if (mode == 0) {

        D_800EBD66 = 512;
        D_800EBD64 = 1024;
    } else if (mode == 1) {

        if (arg0 & 8) {
            graph_env = (u8)(mode + 1);
        }
        D_800EBD64 = 1024;
        D_800EBD66 = 1024;
    } else if (mode == 3) {

        if (arg0 & 8) {
            graph_env = graph_env + 1;
            D_800EBD64 = 1024;
            D_800EBD66 = 1024;
            graph_jtb->func_10(0x09000001);
        } else {
            D_800EBD66 = 512;
            D_800EBD64 = 1024;
        }
    }

    memset(libgpu_cur_drawenv, -1, 92);
    memset(libgpu_graph_disp_env_cache, -1, 20);
}
