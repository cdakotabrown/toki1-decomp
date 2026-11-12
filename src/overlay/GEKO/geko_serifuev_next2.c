/* geko_serifuev_next2 -- GEKO overlay, Address: 0x8013FAE8, Size: 120 bytes (30 insns), Matched */
#include "types.h"

extern void geko_serifuev_run(void);
extern void sub_sub_step_up(void);

extern u16 now_serifuev;
extern s16 now_serifuev_in;
extern s16 sound_seek_time;
extern u8 yazirushi_cw_disp;
extern s32 serifu_state;
extern s32 serifu_voice;

void geko_serifuev_next2(void)
{
    geko_serifuev_run();
    if (serifu_state == 0) {
        return;
    }
    serifu_state = 0;
    serifu_voice = 0;
    yazirushi_cw_disp = yazirushi_cw_disp & 0xFF7F;
    sound_seek_time = 0;
    now_serifuev = now_serifuev + 1;
    now_serifuev_in = 0;
    sub_sub_step_up();
}
