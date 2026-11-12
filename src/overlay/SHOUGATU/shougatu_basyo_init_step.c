/* @SymbolStart: 0x80133698 */
/* @SymbolSize: 120 */
/* Address: 0x801336A0 */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void k_reset(s32);
extern void basyo_init(void *);
extern void dec_bg_cache_step(s32);
extern s32 strcpy(void *, void *);
extern u8 ba_string;
extern u8 shougatu_basyo_init_step_str0;
extern volatile u8 now_sub_sub_step;
extern u8 shougatu_girl_found;

void shougatu_basyo_init_step(void)
{
    if (shougatu_girl_found == 0) {
        dec_bg_cache_step(16743);
        strcpy((void *)&ba_string, (void *)&shougatu_basyo_init_step_str0);
        k_reset(0);
        basyo_init((void *)&ba_string);
    } else {
        now_sub_sub_step = now_sub_sub_step + 3;
    }
    sub_sub_step_up();
}
