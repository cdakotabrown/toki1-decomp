/* Address: 0x8013484C */
/* Size: 276 bytes (69 instructions) */
/* @SymbolSize: 276 */
#include "types.h"

extern void * etc_spr46_res;
extern void * etc_spr46_res_cels;
extern void * etc_spr46_res_anim_tbl;
extern s16 etc_spr46_res_scale_h;
extern void * etc_spr46_res_pal_summer;
extern void * etc_spr46_res_pal_other;
extern void * etc_spr46_res_bg_summer0;
extern void * etc_spr46_res_bg_summer1;
extern void * etc_spr46_res_bg_summer2;
extern void * etc_spr46_res_bg_summer3;
extern void * etc_spr46_res_bg_summer4;
extern void * etc_spr46_res_bg_summer5;
extern void * etc_spr46_res_bg_other0;
extern void * etc_spr46_res_bg_other1;
extern void * etc_spr46_res_bg_other2;
extern void * etc_spr46_res_bg_other3;
extern void * etc_spr46_res_bg_other4;
extern void * etc_spr46_res_bg_other5;

void etc_spr46_res_set(void)
{
    etc_spr46_res = (void *)0x801E8088;
    etc_spr46_res_cels = (void *)0x801E808C;
    etc_spr46_res_anim_tbl = (void *)0x801E80A0;
    etc_spr46_res_scale_h = *(s16 *)0x801E80B4;
    etc_spr46_res_pal_summer = (void *)0x801B0000;
    etc_spr46_res_pal_other = (void *)0x801B2000;
    etc_spr46_res_bg_summer0 = (void *)0x801B4000;
    etc_spr46_res_bg_summer1 = (void *)0x801B8000;
    etc_spr46_res_bg_summer2 = (void *)0x801BC000;
    etc_spr46_res_bg_summer3 = (void *)0x801C0000;
    etc_spr46_res_bg_summer4 = (void *)0x801C4000;
    etc_spr46_res_bg_summer5 = (void *)0x801C8000;
    etc_spr46_res_bg_other0 = (void *)0x801D0000;
    etc_spr46_res_bg_other1 = (void *)0x801D4000;
    etc_spr46_res_bg_other2 = (void *)0x801D8000;
    etc_spr46_res_bg_other3 = (void *)0x801DC000;
    etc_spr46_res_bg_other4 = (void *)0x801E0000;
    etc_spr46_res_bg_other5 = (void *)0x801E4000;
}
