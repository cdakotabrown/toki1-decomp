/* Address: 0x800429B8 */
/* Size: 68 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
#include "types.h"

extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 step_state;
extern s32 step_counter;
extern u8 scene_entry_c;
extern u8 step_mode;

s32 sub_step_up(void)
{
    now_sub_step = now_sub_step + 1;
    now_sub_sub_step = 0;
    step_state = 0;
    step_counter = 0;
    scene_entry_c = 0;
    step_mode = 0;
    return 0;
}
