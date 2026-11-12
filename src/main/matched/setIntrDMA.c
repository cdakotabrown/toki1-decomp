/* Function: setIntrDMA */
/* Address: 0x800B4630 */
/* Size: 256 bytes (64 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -mcpu=4000 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 libapi_dma_callback_tbl[];
extern u16 libapi_dma_intr_enabled;
extern u32 *libetc_dma_dicr_ptr;

extern s32 SetIntrMask(s32 arg0);

s32 setIntrDMA(s32 idx, s32 new_val) {
    s32 old_val;
    s32 saved_result;
    u32 *ctrl;
    u32 m;
    u32 bits;

    old_val = libapi_dma_callback_tbl[idx];

    if (new_val == old_val) {
        return old_val;
    }

    if (libapi_dma_intr_enabled == 0) {
        return old_val;
    }

    saved_result = SetIntrMask(0);

    if (new_val != 0) {
        ctrl = libetc_dma_dicr_ptr;
        libapi_dma_callback_tbl[idx] = new_val;
        m = *ctrl & 0x00FFFFFF;
        bits = ((u32)1 << (idx + 16)) | 0x00800000;
        *ctrl = m | bits;
    } else {
        ctrl = libetc_dma_dicr_ptr;
        libapi_dma_callback_tbl[idx] = 0;
        *ctrl = ((*ctrl & 0x00FFFFFF) | 0x00800000) & ~((u32)1 << (idx + 16));
    }

    SetIntrMask(saved_result);

    return old_val;
}
