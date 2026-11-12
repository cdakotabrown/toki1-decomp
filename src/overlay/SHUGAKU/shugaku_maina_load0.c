/* @SymbolSize: 56 */
/* shugaku_maina_load0 - SHUGAKU overlay, Address: 0x80134E58, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a, s32 b, s32 c);
extern void shugaku_ptr_init(void);
extern void sub_sub_step_up(void);

void shugaku_maina_load0(void)
{
    cd_read_start2(118, 0x801B0000, 0x8D47);
    shugaku_ptr_init();
    sub_sub_step_up();
}
