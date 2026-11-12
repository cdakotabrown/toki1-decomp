/* Address: 0x800B44D0 */
/* Size: 352 bytes (88 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x104 */
#include "types.h"

typedef void (*Cb)(void);

extern volatile u32 *libetc_dma_dicr_ptr;
extern Cb volatile libapi_dma_callback_tbl[];
extern u8 libapi_dma_bus_error_fmt[];

extern void printf(u8 *msg, u32 v);

void trapIntrDMA(void)
{
    u32 mask;
    s32 i;
    u32 dead;
    u32 lo;

    while ((mask = (*libetc_dma_dicr_ptr >> 24) & 0x7F) != 0) {
        lo = 0x00FFFFFF;
        for (i = 0; mask != 0 && i < 7; i++, mask >>= 1) {
            if (mask & 1) {
                *libetc_dma_dicr_ptr &= ((1 << (i + 24)) | lo);
                dead = *libetc_dma_dicr_ptr;
                if (libapi_dma_callback_tbl[i] != 0) {
                    libapi_dma_callback_tbl[i]();
                }
            }
        }
    }

    if ((*libetc_dma_dicr_ptr & 0xFF000000) == 0x80000000 ||
        (*libetc_dma_dicr_ptr & 0x8000) != 0) {
        printf(libapi_dma_bus_error_fmt, *libetc_dma_dicr_ptr);
    }
}
