/* @SymbolStart: 0x80137AF0 */
/* @SymbolSize: 164 */
/* date2_ptr_swap - DATE2 overlay, Address: 0x80137AF0 (prologue 0x80137B6C), */
/* Size: 164 bytes (41 insns).  Entered at 0x80137AF0 through the */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern u8 now_year;
extern s32 date2_serifu_save_ptr[];
extern u32 date2_res_tbl2[][13];

void date2_ptr_swap(void)
{
    date2_serifu_save_ptr[0] = ss_serifu;
    date2_serifu_save_ptr[1] = ss_serifu_kind;
    date2_serifu_save_ptr[2] = sss_sound;
    now_serifu = 0;
    now_serifu_in = now_serifu_in + now_year - 95;
    ss_serifu = (*(s32 *)&date2_res_tbl2[0][11]);
    ss_serifu_kind = (*(s32 *)&date2_res_tbl2[1][11]);
    sss_sound = (*(s32 *)&date2_res_tbl2[2][11]);
    sub_sub_step_up();
}
