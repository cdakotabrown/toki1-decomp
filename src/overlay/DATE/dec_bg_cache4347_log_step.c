/* dec_bg_cache4347_log_step - DATE overlay, Address: 0x80153A88, Size: 76 bytes (19 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32 a);
extern void event_log_add(s32 a, s32 b);
extern void sub_sub_step_up(void);
extern u8 rec_idx;
extern u8 now_date_spot;

void dec_bg_cache4347_log_step(void)
{
    dec_bg_cache_step(17223);
    rec_idx--;
    event_log_add(0, now_date_spot);
    sub_sub_step_up();
}
