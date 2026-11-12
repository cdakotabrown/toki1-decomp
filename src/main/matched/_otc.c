/* Function: _otc */
/* Address: 0x800A1504 */
/* Size: 216 bytes (54 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_BRANCH_FOLD=0 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"
extern s32 *_qlog; extern s32 *libgpu_dma6_bcr_ptr; extern s32 *libgpu_dma6_chcr_reg_ptr;
extern s32 * volatile libgpu_dma_dpcr_reg_ptr;
extern void set_alarm(void); extern s32 get_alarm(void);
s32 _otc(s32 base, s32 count)
{
    s32 *ctrl;
    s32 *status;

    ctrl = libgpu_dma_dpcr_reg_ptr;
    status = libgpu_dma6_chcr_reg_ptr;
    *ctrl = *ctrl | 0x08000000;
    *status = 0;
    *_qlog = base - 4 + (count << 2);
    *libgpu_dma6_bcr_ptr = count;
    *status = 0x11000002;
    set_alarm();
    if ((*libgpu_dma6_chcr_reg_ptr & 0x01000000) == 0) {
        goto done;
    }
loop:
    if (get_alarm() != 0) {
        return -1;
    }
    if ((*libgpu_dma6_chcr_reg_ptr & 0x01000000) != 0) {
        goto loop;
    }
done:
    return count;
}
