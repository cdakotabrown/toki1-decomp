/* @SymbolStart: 0x801375DC */
/* @SymbolSize: 100 */
/* geko_serifu_tbl_restore -- GEKO overlay, Address: 0x80137610, Size: 48 bytes (12 insns) */
#include "types.h"
extern s16 geko_last_serifu2;
extern s16 geko_now_serifu2;
extern s32 geko_last_ss_serifu2;
extern s32 geko_ss_serifu2;
extern s32 geko_last_ss_serifu2_kind;
extern s32 geko_last_sss_sound2;

extern void geko_name_set(void);
extern void sub_sub_step_up(void);

extern s32 geko_ss_serifu2_kind;
extern s32 geko_sss_sound2;

void geko_serifu_tbl_restore(void)
{
    s32 t8;
    s32 t9;

    geko_now_serifu2 = geko_last_serifu2;
    geko_ss_serifu2 = geko_last_ss_serifu2;
    t8 = geko_last_ss_serifu2_kind;
    t9 = geko_last_sss_sound2;

    geko_ss_serifu2_kind = t8;
    geko_sss_sound2 = t9;
    geko_name_set();
    sub_sub_step_up();

}
