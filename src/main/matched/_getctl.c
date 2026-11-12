/* Function: _getctl */
/* Address: 0x800A1D44 */
/* Size: 28 bytes (7 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x800A1D44 */
/* @SymbolSize: 28 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x0:rt:v1:v0 --patch 0x4:rs:v1:v0 --patch 0x4:rd:v1:v0 --patch 0x8:rs:v1:v0 --patch 0x8:rt:v1:v0 --patch 0x18:rs:v1:v0 */
#include "types.h"

extern u8 libgpu_gp1_shadow_tbl[];

u32 _getctl(u32 n)
{
    u32 r;

    r = libgpu_gp1_shadow_tbl[n];
    return r & 0x00FFFFFF;
}
