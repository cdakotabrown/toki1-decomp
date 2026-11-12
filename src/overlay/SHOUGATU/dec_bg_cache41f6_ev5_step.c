/* @SymbolStart: 0x8013438C */
/* Address: 0x80134394 */
/* Size: 116 bytes (29 instructions) */
/* @SymbolSize: 124 */
#include "types.h"

extern void event_log_add(s32, s32);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

extern s32 frame_counter;
extern u8 shougatu_girl0_good;
extern u8 shougatu_alone_flag;

void dec_bg_cache41f6_ev5_step(void)
{
    if (shougatu_alone_flag == 0 && (u32)frame_counter % 3 == 0) {
        if (shougatu_girl0_good != 0) {
            event_log_add(5, 4);
        } else {
            event_log_add(5, 1);
        }
    }
    dec_bg_cache_step(16886);
    sub_sub_step_up();
}
