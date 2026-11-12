/* geko_sound_reset_step -- GEKO overlay, Address: 0x801419B8, Size: 60 bytes (15 insns) */
#include "types.h"

extern void k_reset(s32 a0);
extern void sub_sub_step_up(void);

extern s16 sound_seek_time;
extern s32 serifu_state;
extern s32 serifu_voice;

void geko_sound_reset_step(void)
{
    k_reset(1);
    serifu_state = 0;
    serifu_voice = 0;
    sound_seek_time = 0;
    sub_sub_step_up();
}
