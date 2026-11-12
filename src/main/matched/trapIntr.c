/* Address: 0x800B3CB0 */
/* Size: 436 bytes (109 instructions) */
/* Region: Library (Psy-Q GCC 2.6.0, -O2) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-load-delay --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xe8 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x158 */
#include "types.h"

typedef struct {
    u16 init;
    u16 inIntr;
    void (* volatile tbl[11])(void);
    volatile s32 mask;
} IntrEnv;

extern IntrEnv libapi_intr_env;
extern u16 *libapi_trapintr_stat_reg_ptr;
extern volatile u16 *_mask;
extern s32 libapi_intr_timeout_count;
extern u8 libapi_intr_timeout_fmt[];
extern void printf(u8 *fmt, ...);
extern void ReturnFromException(void);

void trapIntr(void)
{
    u16 mask;
    s32 i;

    libapi_intr_env.inIntr = 1;
    mask = libapi_intr_env.mask & *libapi_trapintr_stat_reg_ptr & *_mask;
    while (mask) {
        i = 0;
        while (mask && i < 11) {
            if (mask & 1) {
                *libapi_trapintr_stat_reg_ptr = ~(1 << i);
                *(volatile u16 *)libapi_trapintr_stat_reg_ptr;
                if (libapi_intr_env.tbl[i]) {
                    libapi_intr_env.tbl[i]();
                }
            }
            mask >>= 1;
            i++;
        }
        mask = libapi_intr_env.mask & *libapi_trapintr_stat_reg_ptr & *_mask;
    }
    if (*libapi_trapintr_stat_reg_ptr & *_mask) {
        if (libapi_intr_timeout_count++ >= 2049) {
            printf(libapi_intr_timeout_fmt, *libapi_trapintr_stat_reg_ptr, *_mask);
            libapi_intr_timeout_count = 0;
            *libapi_trapintr_stat_reg_ptr = 0;
        }
    } else {
        libapi_intr_timeout_count = 0;
    }
    *(volatile u16 *)&libapi_intr_env.inIntr = 0;
    ReturnFromException();
}
