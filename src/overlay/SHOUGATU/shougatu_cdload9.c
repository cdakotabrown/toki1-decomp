/* Address: 0x8013B81C */
/* Size: 60 bytes (15 instructions) */
/* Size: 0xAFB3 (44979) bytes, then processes scene */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_ptrs1_set(void);

void shougatu_cdload9(void)
{
    cd_read_start2(9, (void *)0x80197000, 0xAFB3);
    shougatu_ptrs1_set();
    sub_sub_step_up();
}
