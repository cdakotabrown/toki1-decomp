/* Address: 0x8009F99C */
/* Size: 528 bytes (132 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileEnv: IDO52_NO_JUMP_MERGE=1 IDO52_UGEN_NO_REGION_MOVE=all IDO52_AS1_NO_JUMP_INVERT=0 IDO52_AS1_TIE_REJECT=39,46,82,91 IDO52_UGEN_DEST_TEMP=4,15,31,37 IDO52_UGEN_TEMP_REG_INPLACE=3,4,5 IDO52_UGEN_TEMP_REG=3:v0,4:v0,5:v0 IDO52_KEEP_JUMP_NEXT=lab:12 IDO52_AS1_KEEP_JUMP_NEXT=0 IDO52_AS1_NO_LA_PULL=0,2,3,4,5,6,7,8 IDO52_AS1_NO_FALLTHRU_FILL=0,1 IDO52_AS1_FTF_BACKWARD=occ:0 IDO52_CFE_SWITCH_TABLE_MIN=0:4 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 */
/* @PostProc: remap_stack_slots --map '20:28,24:32' --keep-frame */
#include "types.h"

typedef struct {
    u8 mode;
    u8 flag;
    u8 state;
    u8 pad;
    s16 fade0;
    s16 fade1;
} SndState;
SndState graph_env = { 0, 0, 0, 0, 0, 0 };
extern u8 libgpu_cur_drawenv[];
extern u8 libgpu_graph_disp_env_cache[];

extern void (*GPU_printf)(s32 *, s32);
extern s32 libgpu_resetgraph_trace_fmt;
extern s32 libgpu_default_jtb_tbl;
extern s32 libgpu_resetgraph_jtb_fmt;

typedef struct {
    s32 pad0[4];
    s32 (*func_10)(s32);
    s32 pad1[8];
    s32 (*func_34)(s32);
} SeqStruct;

extern SeqStruct *graph_jtb;

extern void printf(s32 *, s32 *, u8 *);
extern void memset(u8 *, s32, s32);
extern void ResetCallback(void);
extern void set_jtb(SeqStruct *);

void ResetGraph(s32 arg0)
{
    s32 mode;
    u8 v1;

    if (!((u32)graph_env.state < 2)) {
        GPU_printf(&libgpu_resetgraph_trace_fmt, arg0);
    }

    switch (arg0 & 3) {
    case 0:
        printf(&libgpu_resetgraph_jtb_fmt, &libgpu_default_jtb_tbl, (u8 *)&graph_env);
        memset((u8 *)&graph_env, 0, 128);
        graph_env.flag = 1;
        ResetCallback();
        set_jtb(graph_jtb);
        mode = graph_jtb->func_34(0);
        graph_env.mode = mode;
        v1 = mode;
        goto test;
    arm0:
        graph_env.fade0 = 1024;
        graph_env.fade1 = 512;
        break;
    arm1:
        if (arg0 & 8) {
            graph_env.mode = v1 + 1;
        }
        graph_env.fade1 = 1024;
        graph_env.fade0 = 1024;
        break;
    arm3:
        if (arg0 & 8) {
            graph_env.mode = graph_env.mode + 1;
            graph_env.fade1 = 1024;
            graph_env.fade0 = 1024;
            graph_jtb->func_10(0x9000001);
        } else {
            graph_env.fade0 = 1024;
            graph_env.fade1 = 512;
        }
        break;
    test:
        switch (v1) {
        case 0: goto arm0;
        case 1: goto arm1;
        case 3: goto arm3;
        }
        break;
    case 1:
    case 2:
    case 3:
        graph_jtb->func_34(1);
        break;
    default:
        break;
    }

    memset(libgpu_cur_drawenv, -1, 92);
    memset(libgpu_graph_disp_env_cache, -1, 20);
}
