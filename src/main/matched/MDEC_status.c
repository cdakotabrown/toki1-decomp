/* Address: 0x80086E84 */
/* Size: 24 bytes (6 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 *libpress_mdec_ctrl_reg_ptr;

s32 MDEC_status(void) {
    return *libpress_mdec_ctrl_reg_ptr;
}
