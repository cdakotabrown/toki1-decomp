/* Address: 0x8009D0BC */
/* Size: 100 bytes (25 instructions) */
/* Region: IDO Library (0x80086000+) */
/* Likely Symbol: gte_set_lc */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_ra_offset --old 20 --new 16 */
/* @PostProc: swap_registers v0 t8 */
/* @PostProc: swap_registers a1 v0 */
/* @PostProc: swap_registers a2 a1 */
/* @PostProc: swap_registers t8 a2 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 _LC[];
extern void SetColorMatrix(void);

void gte_set_lc(s32 *src)
{
    volatile s32 *dst;
    volatile s32 *vsrc;
    s32 a, b, c;

    vsrc = src;
    dst = _LC;
    a = vsrc[2]; b = vsrc[1]; c = vsrc[0];
    dst[2] = a; dst[1] = b; dst[0] = c;
    a = vsrc[5]; b = vsrc[4]; c = vsrc[3];
    dst[5] = a; dst[4] = b; dst[3] = c;
    b = vsrc[7]; c = vsrc[6];
    dst[7] = b;
    dst[6] = c;
    SetColorMatrix();
}
