/* Address: 0x800A1D18 */
/* Size: 44 bytes (11 instructions) */
/* Region: Library (IDO, ASPSX-assembled -- see PIPELINE) */
/* PIPELINE: `@Compiler: ido52-gas` + `--ido-fill-load-delay`.  That gives */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0xC --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1C */
/* @PostProc: swap_instructions --offset1 0x1C --offset2 0x20 */
#include "types.h"

extern u32 *GPU_status;
extern u8 libgpu_gp1_shadow_tbl[];

void _ctl(u32 v)
{
    u32 m;

    m = v & 0xFFFFFF;
    *GPU_status = v;
    libgpu_gp1_shadow_tbl[v >> 24] = m;
}
