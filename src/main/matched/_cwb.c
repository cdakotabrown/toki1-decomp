/* Function: _cwb */
/* Address: 0x800A1D60 */
/* Size: 72 bytes (18 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x800A1D60 */
/* @SymbolSize: 72 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=4 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u32 *libgpu_gp0_reg_ptr;
extern u32 *GPU_status;

s32 _cwb(u32 *src, s32 n)
{
    u32 *port;
    s32 old;

    old = n--;
    *GPU_status = 0x04000000;
    if (old) {
        port = libgpu_gp0_reg_ptr;
        do {
            old = n--;
            *port = *src++;
        } while (old);
    }
    return 0;
}
