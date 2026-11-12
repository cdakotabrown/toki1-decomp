/* @SymbolStart: 0x8013CA90 */
/* @SymbolSize: 84 */
/* geko_girl14_name_set -- GEKO overlay, Address: 0x8013CA98, Size: 76 bytes (19 insns) */
#include "types.h"

extern void strcpy(void *a0, void *a1);
extern void sub_sub_step_up(void);

extern s8 bustup_cd_read_tbl0;
extern u8 now_girl;
extern u8 g_name;
extern u8 geko_girl14_name_set_str0;
extern s16 cal_cw_anim;

void geko_girl14_name_set(void)
{
    bustup_cd_read_tbl0 = 1;
    now_girl = 14;
    strcpy(&g_name, &geko_girl14_name_set_str0);
    cal_cw_anim = 4;
    sub_sub_step_up();
}
