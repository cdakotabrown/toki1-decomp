/* @SymbolStart: 0x800A04E0 */
/* @SymbolSize: 232 */
/* @CompileEnv: IDO52_UGEN_DEST_TEMP=3,12 IDO52_FORCE_WEB_COLOR=10:v1,30:v1 IDO52_FORCE_WEB_NOSPILL=10.1 IDO52_AS1_NO_COPY_PROP=5 IDO52_AS1_NO_PASS1_PROP=40 IDO52_UGEN_TEMP_REG=9:t3 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map 32:36 */
/* @CompileFlags: -Wc,-pic0 */
#include "types.h"

typedef struct {
    u32 w[23];
} Blk92;

extern u8 D_800EBD62;
extern void (*GPU_printf)(u8 *, s32);
extern u8 libgpu_putdrawenv_trace_fmt;

typedef struct {
    u32 field_00;
    u32 field_04;
    s32 (*func_08)(s32, s32, s32, s32);
    u32 pad_0C[3];
    s32 field_18;
} ContStruct;

extern ContStruct *graph_jtb;
extern Blk92 libgpu_cur_drawenv;

extern void SetDrawEnv(u32 *arg0, u32 *arg1);

u32 *PutDrawEnv(u32 *arg0) {
    ContStruct *c;

    if (D_800EBD62 >= 2U) {
        GPU_printf(&libgpu_putdrawenv_trace_fmt, (s32)arg0);
    }
    SetDrawEnv(arg0 + 7, arg0);
    c = graph_jtb;
    arg0[7] = (arg0[7] & 0xFF000000) | 0x00FFFFFF;
    c->func_08(c->field_18, (s32)(arg0 + 7), 64, 0);
    libgpu_cur_drawenv = *(Blk92 *)arg0;
    return arg0;
}
