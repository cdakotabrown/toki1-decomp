/* Function: _cwc */
/* Address: 0x800A1DA8 */
/* Size: 68 bytes (17 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x800A1DA8 */
/* @SymbolSize: 68 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u32 *GPU_status;
extern u32 *libgpu_dma2_madr_ptr;
extern u32 *libgpu_dma2_bcr_ptr;
extern u32 *libgpu_dma2_chcr_ptr;

void _cwc(u32 addr)
{
    *GPU_status = 0x04000002;
    *libgpu_dma2_madr_ptr = addr;
    *libgpu_dma2_bcr_ptr = 0;
    *libgpu_dma2_chcr_ptr = 0x01000401;
}
