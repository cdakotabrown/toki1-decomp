/* dec_bg_cache4144_step2 -- GEKO overlay, Address: 0x80140A08, Size: 64 bytes (16 insns) */
#include "types.h"

extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

extern s32 dec_bg_on;

void dec_bg_cache4144_step2(void)
{
    dec_bg_on = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    dec_bg_cache_step(16708);
    sub_sub_step_up();
}
