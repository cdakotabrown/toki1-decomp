/* Address: 0x80134CD0 */
/* Size: 288 bytes (72 instructions) */
/* @SymbolSize: 288 */
#include "types.h"

extern void * etc_spr0e_res;
extern void * etc_spr0e_res_cels;
extern void * etc_spr0e_res_anim_tbl;
extern s16 etc_spr0e_res_scale_h;
extern void * etc_spr0e_res_pal_summer;
extern void * etc_spr0e_res_pal_other;
extern void * etc_spr0e_res_bg_summer0;
extern void * etc_spr0e_res_bg_summer1;
extern void * etc_spr0e_res_bg_summer2;
extern void * etc_spr0e_res_bg_summer3;
extern void * etc_spr0e_res_bg_summer4;
extern void * etc_spr0e_res_bg_summer5;
extern void * etc_spr0e_res_bg_other0;
extern void * etc_spr0e_res_bg_other1;
extern void * etc_spr0e_res_bg_other2;
extern void * etc_spr0e_res_bg_other3;
extern void * etc_spr0e_res_bg_other4;
extern void * etc_spr0e_res_bg_other5;

void etc_spr0e_res_set(void)
{
    etc_spr0e_res = (void *)0x801E60CC;
    etc_spr0e_res_cels = (void *)0x801E60D0;
    etc_spr0e_res_anim_tbl = (void *)0x801E60E4;
    etc_spr0e_res_scale_h = *(s16 *)0x801E60F0;
    etc_spr0e_res_pal_summer = (void *)0x801AE000;
    etc_spr0e_res_pal_other = (void *)0x801B0000;
    etc_spr0e_res_bg_summer0 = (void *)0x801B2000;
    etc_spr0e_res_bg_summer1 = (void *)0x801B6000;
    etc_spr0e_res_bg_summer2 = (void *)0x801BA000;
    etc_spr0e_res_bg_summer3 = (void *)0x801BE000;
    etc_spr0e_res_bg_summer4 = (void *)0x801C2000;
    etc_spr0e_res_bg_summer5 = (void *)0x801C6000;
    etc_spr0e_res_bg_other0 = (void *)0x801CE000;
    etc_spr0e_res_bg_other1 = (void *)0x801D2000;
    etc_spr0e_res_bg_other2 = (void *)0x801D6000;
    etc_spr0e_res_bg_other3 = (void *)0x801DA000;
    etc_spr0e_res_bg_other4 = (void *)0x801DE000;
    etc_spr0e_res_bg_other5 = (void *)0x801E2000;
}
