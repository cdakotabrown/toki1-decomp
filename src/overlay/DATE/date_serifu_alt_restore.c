/* @SymbolStart: 0x80149C10 */
/* @SymbolSize: 100 */
/* date_serifu_alt_restore - DATE overlay, Address: 0x80149C44, Size: 96 bytes (24 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void date_msg_set(void);
extern s16 date_now_serifu2;
extern s16 date_now_serifu2_save;
extern s32 date_ss_serifu2;
extern s32 date_ss_serifu2_kind;
extern s32 date_sss_sound2;
extern s32 date_serifu2_save_ptr[];

void date_serifu_alt_restore(void)
{
    date_now_serifu2 = date_now_serifu2_save;
    date_ss_serifu2 = date_serifu2_save_ptr[0];
    date_ss_serifu2_kind = date_serifu2_save_ptr[1];
    date_sss_sound2 = date_serifu2_save_ptr[2];
    date_msg_set();
    sub_sub_step_up();
}
