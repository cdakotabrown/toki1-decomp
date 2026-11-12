/* kangei_choice_set - KANGEI overlay, Address: 0x801362BC, Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 serifu_no;
extern u8 kangei_choice_no;
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void kangei_choice_set(void)
{
    device_cmd_send(1280);
    kangei_choice_no = (u8)serifu_no;
    sub_sub_step_up();
}
