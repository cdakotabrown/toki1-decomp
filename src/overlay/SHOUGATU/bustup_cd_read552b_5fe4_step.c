/* @SymbolStart: 0x8013A5B0 */
/* @SymbolSize: 72 */
/* bustup_cd_read552b_5fe4_step - SHOUGATU overlay, Address: 0x8013A5B8, Size: 80 bytes (20 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void bustup_cd_read();
extern u8 now_girl;

void bustup_cd_read552b_5fe4_step(void)
{
    if (now_girl == 5) {
        bustup_cd_read(24548);
    } else {
        bustup_cd_read(21803);
    }
    sub_sub_step_up();
}
