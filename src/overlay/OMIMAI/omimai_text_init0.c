/* omimai_text_init0 - OMIMAI overlay, Address: 0x80132630, Size: 64 bytes (16 insns) */
#include "types.h"

extern void text_sys_init(void);
extern void k_reset(s32);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void sub_sub_step_up(void);

void omimai_text_init0(void)
{
    text_sys_init();
    k_reset(1);
    bustup_ptr_init();
    yazirushi_off();
    sub_sub_step_up();
}
