/* kangei_choice_skip_step -- KANGEI overlay, Address: 0x80136940, Size: 56 bytes (14 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 now_sub_sub_step;
extern s32 serifu_no;

void kangei_choice_skip_step(void)
{
    s32 t6 = serifu_no;

    if (t6 == 0) goto label0;
    now_sub_sub_step = (now_sub_sub_step + 19);

label0:
    sub_sub_step_up();
}
