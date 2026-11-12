/* Function: date_girl3_mode2_xa_cue_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_xa_cue_ptr_set(void);

void date_girl3_mode2_xa_cue_load(void)
{
    cd_read_start2(1, 0x801E4000, 0x8FF2);
    date_xa_cue_ptr_set();
    sub_sub_step_up();
}
