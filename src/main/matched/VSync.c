/* Address: 0x800B4020 */
/* Size: 324 bytes (81 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-all --label-load-nop --macro-lui-fill */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:v0 0x28:rt:v0:v1 0x2c:rs:v0:v1 0x2c:rt:v0:v1 0x34:rs:v1:v0 0x34:rt:v0:v1 */
/* @PostProc: patch_insn_reg_field 0xcc:rt:a0:v1 0xd0:rs:a0:v1 0xd0:rt:a0:v1 0xd8:rs:a0:v1 0xd8:rt:v1:v0 0xe0:rt:v1:v0 0xec:rt:a1:a0 0xf0:rs:a0:v1 0xf0:rt:v1:v0 0xf8:rt:v1:v0 0xfc:rt:a1:a0 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x4 */
#include "types.h"

extern volatile s32 *libapi_vsync_gpustat_ptr;
extern volatile s32 *libapi_vsync_rcnt1_count_ptr;
extern volatile s32 Hcount;
extern s32 libapi_vsync_last_vcount;
extern volatile s32 Vcount;

extern void v_wait(s32, s32);

s32 VSync(s32 arg0) {
    volatile s32 *ptrA;
    s32 s0val;
    s32 valB;
    s32 masked;
    s32 idx;

    s0val = *libapi_vsync_gpustat_ptr;
    valB = *libapi_vsync_rcnt1_count_ptr;
    masked = (valB - Hcount) & 0xFFFF;

    if (arg0 < 0) {
        return Vcount;
    }

    if (arg0 == 1) {
        return masked;
    }

    if (arg0 > 0) {
        idx = libapi_vsync_last_vcount - (1 - arg0);
    } else {
        idx = libapi_vsync_last_vcount;
    }
    v_wait(idx, arg0 > 0 ? arg0 - 1 : 0);

    s0val = *libapi_vsync_gpustat_ptr;
    v_wait(Vcount + 1, 1);

    if (s0val & 0x00080000) {
        ptrA = libapi_vsync_gpustat_ptr;
        valB = *ptrA;
        if ((s32)(s0val ^ valB) >= 0) {
            do {
                valB = *ptrA;
            } while (((s0val ^ valB) & 0x80000000) == 0);
        }
    }

    libapi_vsync_last_vcount = Vcount;
    Hcount = *libapi_vsync_rcnt1_count_ptr;

    return masked;
}
