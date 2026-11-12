/* Address: 0x800B3BB0, Size: 20 bytes (5 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers t6:v0 --unidirectional */
#include "types.h"

extern volatile u16 libapi_intr_in_intr_flag[];

u16 CheckCallback(void) {
    return libapi_intr_in_intr_flag[0];
}
