/* @SymbolStart: 0x801542EC */
/* @SymbolSize: 112 */
/* Address: 0x801542EC (cross-prologue head) / 0x801542F4 (prologue) */
/* Size: 112 bytes (28 insns) */
#include "types.h"

extern u8 taco_addr_lo_fmt[];
extern u8 taco_addr_hi_fmt[];

extern void printf();

s32 taco_addr_check(s32 *ptr)
{
    if ((u32)ptr < 0x8018A000U) {
        printf(taco_addr_lo_fmt, *ptr);
        return 0;
    }
    if ((u32)ptr >= 0x801B8001U) {
        printf(taco_addr_hi_fmt, *ptr);
        return 0;
    }
    return 1;
}
