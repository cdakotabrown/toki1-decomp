/* shugaku_girl2_event_load - SHUGAKU overlay, Address: 0x801359AC, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void shugaku_desc_init(void);
extern void sub_sub_step_up(void);

void shugaku_girl2_event_load(void)
{
    cd_read_start2(0x25, 0x801B0000, 0x8240);
    shugaku_desc_init();
    sub_sub_step_up();
}
