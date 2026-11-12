/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x5c:rt:t0:t1 */
#include "types.h"

extern u32 date_ev37_sub_res;
extern u32 date_ev37_sub_res_img;
extern volatile u32 date_ev37_sub_res_anim_tbl;
extern volatile s16 date_ev37_sub_res_aux;
extern volatile s16 date_ev37_sub_cel_aux;
extern u32 date_ev37_sub_res_pal;

void date_girl8_mode2_sub_res_ptr_set(void)
{
    date_ev37_sub_res = 0x801D2074;
    date_ev37_sub_res_img = 0x801D2078;
    date_ev37_sub_res_anim_tbl = 0x801D2088;
    date_ev37_sub_res_aux = date_ev37_sub_cel_aux;
    date_ev37_sub_res_pal = 0x801D0000;
}
