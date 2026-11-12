/* Function: SetTexWindow */
/* Address:  0x800A0BE8 */
/* Size:     84 bytes (21 instructions) */
/* Region:   IDO (0x80086000+) - IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers_auto --swaps a2:v1 */
/* @PostProc: swap_registers_auto --swaps t7:t8 */
/* @PostProc: swap_registers_auto --swaps t6:t7 */
/* @PostProc: swap_registers_liveness --target-exe disks/SLPS_000.65 --address 0x800A0BE8 */
/* Function: Modifies structure header and calls get_tw */
#include "types.h"

extern s32 get_tw(s32 arg);

void SetTexWindow(u32 *ptr, s32 arg) {
    volatile u32 *p = ptr;
    u32 val;
    u32 masked;
    u32 combined;
    s32 result;

    val = *p;
    masked = val & 0x00FFFFFF;
    combined = masked | 0x02000000;
    *p = combined;
    result = get_tw(arg);
    ((u32*)p)[1] = result;
    ((u32*)p)[2] = 0;
}
