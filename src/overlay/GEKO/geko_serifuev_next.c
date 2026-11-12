/* @SymbolSize: 196 */
/* Address: 0x8013BE0C, Size: 196 bytes (49 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_l_com(void);
extern void k_reset(s32 a0);
extern s32 check_end_k(void);
extern void geko_serifuev_run(void);
extern u16 now_serifuev;
extern u16 now_serifuev_in;
extern s16 sound_seek_time;
extern u8 voice_level;
extern u8 yazirushi_cw_disp;
extern s32 serifu_state;
extern s32 serifu_voice;
volatile char geko_serifuev_next(void)
{
  u8 v0;
  geko_serifuev_run();
  if (now_serifuev_in != 9)
  {
    return;
  }
  if (check_end_k() == 0)
  {
    return;
  }
  v0 = voice_level;
  if (v0 == 0)
  {
    goto do_work;
  }
  if (!v0)
  {
    return;
  }
  if (get_CD_l_com() != 9)
  {
    return;
  }
  do_work:
  serifu_state = 0;

  k_reset(1);
  serifu_voice = 0;
  yazirushi_cw_disp &= 0xFF7F;
  sound_seek_time = 0;
  now_serifuev += 1;
  now_serifuev_in = 0;
  sub_sub_step_up();
}
