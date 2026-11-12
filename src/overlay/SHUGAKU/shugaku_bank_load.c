/* shugaku_bank_load -- SHUGAKU overlay, Address: 0x80137C60, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void shugaku_bank_ptr_init(void);
extern void sub_sub_step_up(void);

void shugaku_bank_load(void)
{
    cd_read_start2(45, 0x80197000, 0xAF7C);
    shugaku_bank_ptr_init();
    sub_sub_step_up();
}
