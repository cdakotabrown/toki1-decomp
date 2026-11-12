/* @SymbolStart: 0x801379CC */
/* @SymbolSize: 96 */
/* date2_ptr_restore - DATE2 overlay, Address: 0x801379FC, Size: 48 bytes (12 insns) */
#include "types.h"

extern s16 now_serifu;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 date2_serifu_save_ptr[];

void date2_msg_set(void);
void sub_sub_step_up(void);

void date2_ptr_restore(void)
{
    now_serifu = 29;
    ss_serifu = date2_serifu_save_ptr[0];
    ss_serifu_kind = date2_serifu_save_ptr[1];
    sss_sound = date2_serifu_save_ptr[2];
    date2_msg_set();
    sub_sub_step_up();
}
