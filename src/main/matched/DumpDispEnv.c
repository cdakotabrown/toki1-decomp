/* Address: 0x800A3274 */
/* Size: 160 bytes (40 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers t9:t7 --only 0x10,0x14,0x30 */
/* @PostProc: swap_registers t7:t8 --only 0x40,0x5c */
/* @PostProc: swap_registers t9:t0 --only 0x60,0x64,0x70 */
/* @PostProc: swap_registers t9:t1 --only 0x78,0x7c,0x88 */
#include "types.h"

typedef struct {
    s16 field_00;
    s16 field_02;
    s16 field_04;
    s16 field_06;
    s16 field_08;
    s16 field_0A;
    s16 field_0C;
    s16 field_0E;
    u8  field_10;
    u8  field_11;
} SoundStruct;

extern void (*GPU_printf)();
extern s32 libgpu_dump_disp_rect_fmt;
extern s32 libgpu_dump_screen_rect_fmt;
extern s32 libgpu_dump_isinter_fmt;
extern s32 libgpu_dump_isrgb24_fmt;

void DumpDispEnv(SoundStruct *arg0) {
    GPU_printf((s32)&libgpu_dump_disp_rect_fmt, arg0->field_00, arg0->field_02, arg0->field_04, arg0->field_06);
    GPU_printf((s32)&libgpu_dump_screen_rect_fmt, arg0->field_08, arg0->field_0A, arg0->field_0C, arg0->field_0E);
    GPU_printf((s32)&libgpu_dump_isinter_fmt, (s32)arg0->field_10);
    GPU_printf((s32)&libgpu_dump_isrgb24_fmt, (s32)arg0->field_11);
}
