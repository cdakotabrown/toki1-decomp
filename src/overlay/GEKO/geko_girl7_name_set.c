/* @CompileEnv: IDO52_CONST_INREG_VALUES=7 */
/* @SymbolStart: 0x8013CB20 */
/* @SymbolSize: 64 */
/* geko_girl7_name_set -- GEKO overlay, Address: 0x8013CB24, Size: 60 bytes */
#include "types.h"
extern void date_name_set(s32 a0);
extern void sub_sub_step_up(void);
extern s8 bustup_cd_read_tbl0;
extern s8 now_girl;
extern s16 cal_cw_anim;
void geko_girl7_name_set(void)
{
    bustup_cd_read_tbl0 = 0;
    date_name_set(now_girl = 7);
    cal_cw_anim = 2;
    sub_sub_step_up();
}
