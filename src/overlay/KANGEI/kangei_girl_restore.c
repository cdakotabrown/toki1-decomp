/* kangei_girl_restore - KANGEI overlay, Address: 0x80133C9C, Size: 36 bytes (9 insns) */
#include "types.h"

extern void sub_step_up(void);

extern u8 now_girl;
extern u8 girl_no;

void
kangei_girl_restore(void)
{
    u8 hiddenT6 = girl_no;

    now_girl = hiddenT6;
    sub_step_up();
}
