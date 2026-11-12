/* dec_bg_cache42eb_step -- GEKO overlay, Address: 0x801415B8, Size: 64 bytes (16 insns) */
#include "types.h"

extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

extern s32 dec_bg_on;

void dec_bg_cache42eb_step(void)
{
    dec_bg_on = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    dec_bg_cache_step(17131);
    sub_sub_step_up();
}
