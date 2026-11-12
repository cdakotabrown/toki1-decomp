/* @SymbolSize: 92 */
/* Address: 0x80134BF0 */
/* Size: 720 bytes (180 instructions) -- TARGET: 180, COMPILED: 23 */
#include "types.h"

extern void geko_ptr_table_init1(void);
extern void date_name_set(s32 a0);
extern void sub_sub_step_up(void);

extern s32 geko_ss_serifu_icon;
extern s32 geko_ss_serifu_icon_kind;
extern s32 geko_sss_sound_icon;
extern s32 geko_serifu1_tbl[];
extern s32 geko_serifu1_kind_tbl[];
extern s32 geko_sound1_tbl[];

void geko_init_step(void)
{
    geko_ptr_table_init1();
    date_name_set(0);
    geko_ss_serifu_icon = geko_serifu1_tbl[11];
    geko_ss_serifu_icon_kind = geko_serifu1_kind_tbl[11];
    geko_sss_sound_icon = geko_sound1_tbl[11];
    sub_sub_step_up();
}
