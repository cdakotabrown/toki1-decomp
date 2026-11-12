/* Function: _param */
/* Address: 0x800A1DEC */
/* Size: 48 bytes (12 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x800A1DEC */
/* @SymbolSize: 48 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x20:rt:v1:v0 --patch 0x2C:rs:v1:v0 */
#include "types.h"

extern u32 *libgpu_gp0_reg_ptr;
extern u32 *GPU_status;

u32 _param(u32 n)
{
    u32 r;

    *GPU_status = n | 0x10000000;
    r = *libgpu_gp0_reg_ptr;
    return r & 0x00FFFFFF;
}
