/* Address: 0x800A0614 */
/* Size: 1292 bytes (323 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (as1-scheduled) */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_MOVE_LOOP_SHIFT=2 IDO52_CONST_NOINREG_VALUES=10 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=1,2,3,4,5,6,7,8,9,10,11,12,13 IDO52_UGEN_KEEP_JUMP_OVER=all IDO52_FORCE_WEB_COLOR=55:t1,80:t0 IDO52_UGEN_DEST_TEMP=4,23,77,106 IDO52_UGEN_TEMP_REG=18:t1,3:t4,6:t3,48:t9,47:t7,12:t3 IDO52_UGEN_TEMP_REG_INPLACE=occ:18 IDO52_AS1_MUL_CONST=aspsx IDO52_AS1_KEEP_DEAD_RELOAD=all IDO52_AS1_NO_JUMP_CHAIN=26,34 IDO52_AS1_NO_PASS1_PROP=70,197,248 IDO52_AS1_TIE_TAKE=23,30,64 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --map 44:52,48:56 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: patch_insn_reg_field --patch 0x90:rs:t4:t3 --patch 0x90:rt:t3:t4 --patch 0x190:rs:v1:a3 --patch 0x190:rt:a3:v1 */
#include "types.h"

typedef struct {
    RECT disp;
    RECT screen;
    u8 isinter;
    u8 isrgb24;
    u8 pad0;
    u8 pad1;
} DISPENV;

typedef struct {
    u8 version;
    u8 pad1;
    u8 level;
    u8 reverse;
    u8 pad4[0x68];
    DISPENV disp;
} GpuInfo;

typedef struct {
    u8 pad[16];
    void (*ctl)(u32);
} GpuFuncs;

extern GpuInfo graph_env;
extern GpuFuncs *graph_jtb;
extern void (*GPU_printf)(u8 *, DISPENV *);
extern u8 libgpu_putdispenv_trace_fmt;
extern s32 GetVideoMode(void);
extern u32 get_dx(DISPENV *env);

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

DISPENV *PutDispEnv(DISPENV *env)
{
    u32 mode;
    s32 h_start, h_end;
    s32 v_start, v_end;

    mode = 0x08000000;
    if (graph_env.level >= 2U) {
        GPU_printf(&libgpu_putdispenv_trace_fmt, env);
    }
    graph_jtb->ctl(((graph_env.version == 1 || graph_env.version == 2)
                     ? ((env->disp.y & 0xFFF) << 12) | (get_dx(env) & 0xFFF)
                     : ((env->disp.y & 0x3FF) << 10) | (env->disp.x & 0x3FF))
                    | 0x05000000);
    if (*(s32 *)&graph_env.disp.screen.x != *(s32 *)&env->screen.x
        || *(s32 *)&graph_env.disp.screen.w != *(s32 *)&env->screen.w) {
        env->pad0 = GetVideoMode();
        h_start = env->screen.x * 10 + 608;
        v_start = (env->pad0 ? 19 : 16) + env->screen.y;
        h_end = (env->screen.w ? env->screen.w : 256) * 10;
        h_end = h_start + h_end;
        v_end = v_start + (env->screen.h ? env->screen.h : 240);
        h_start = CLAMP(h_start, 500, 3290);
        h_end = CLAMP(h_end, h_start + 80, 3290);
        v_start = CLAMP(v_start, 0, (env->pad0 ? 310 : 254));
        v_end = CLAMP(v_end, v_start + 1, (env->pad0 ? 312 : 256));
        graph_jtb->ctl(((h_end & 0xFFF) << 12) | 0x06000000 | (h_start & 0xFFF));
        graph_jtb->ctl(((v_end & 0x3FF) << 10) | 0x07000000 | (v_start & 0x3FF));
    }
    if (*(s32 *)&graph_env.disp.isinter != *(s32 *)&env->isinter
        || *(s32 *)&graph_env.disp.disp.x != *(s32 *)&env->disp.x
        || *(s32 *)&graph_env.disp.disp.w != *(s32 *)&env->disp.w) {
        env->pad0 = GetVideoMode();
        if (env->pad0 == 1) {
            mode |= 8;
        }
        if (env->isrgb24) {
            mode |= 0x10;
        }
        if (env->isinter) {
            mode |= 0x20;
        }
        if (graph_env.reverse) {
            mode |= 0x80;
        }
        if (env->disp.w < 281) {
            mode |= 0;
        } else if (env->disp.w < 353) {
            mode |= 1;
        } else if (env->disp.w < 401) {
            mode |= 0x40;
        } else if (env->disp.w < 561) {
            mode |= 2;
        } else {
            mode |= 3;
        }
        if (env->disp.h <= (env->pad0 ? 288 : 256)) {
            mode |= 0;
        } else {
            mode |= 0x24;
        }
        graph_jtb->ctl(mode);
    }
    graph_env.disp = *env;
    return env;
}
