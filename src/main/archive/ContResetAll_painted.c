/* Function: func_800A04E8 */
/* Address: 0x800A04E8 */
/* Size: 224 bytes (56 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_ContResetAll.py */
#include "types.h"

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
extern u32 libgpu_cur_drawenv[];

extern void SetDrawEnv(u32 *arg0, u32 *arg1);

u32 *func_800A04E8(u32 *arg0) {
    u32 *dest;
    u32 *src;
    u32 val;
    ContStruct *sptr;

    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf(&libgpu_putdrawenv_trace_fmt, (s32)arg0);
    }

    SetDrawEnv(arg0 + 7, arg0);

    val = arg0[7];
    val = (val & 0xFF000000) | 0x00FFFFFF;
    arg0[7] = val;

    sptr = graph_jtb;
    sptr->func_08(sptr->field_18, (s32)(arg0 + 7), 64, 0);

    src = arg0;
    dest = libgpu_cur_drawenv;

    do {
        *dest++ = *src++;
        *dest++ = *src++;
        *dest++ = *src++;
    } while (src != &arg0[21]);
    *dest = *src;
    *(dest + 1) = *(src + 1);

    return arg0;
}
