/* Address: 0x8006D544 */
/* Size: 140 bytes (35 instructions) */
/* Region: Game */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer --target disks/SLPS_000.65 --address 0x8006D544 */
#include "types.h"

extern u8 bukatu_flag0;
extern u8 bukatu_flag1;
extern u8 bukatu_flag2;

extern u8 bukatu_tbl;
extern u8 bukatu_tbl_end;

void bukatu_reset(void) {
    u8 *end;
    u8 *base;

    bukatu_flag0 &= 0xFFFE;
    bukatu_flag1 &= 0xFFFE;
    bukatu_flag2 &= 0xFFFE;

    end = &bukatu_tbl_end;
    base = &bukatu_tbl;

    do {

        base[0x66f] &= 0xFFFE;
        base[0x673] &= 0xFFFE;
        base[0x677] &= 0xFFFE;
        base[0x67b] &= 0xFFFE;
        base += 16;
    } while (base != end);
}
