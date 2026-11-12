/* Address: 0x8013DE60 */
/* Size: 64 bytes (16 instructions) */
/* @SymbolSize: 64 */
#include "types.h"

extern void * shougatu_cal_cw_res;
extern void * shougatu_cal_cw_res_1;
extern void * shougatu_cal_cw_res_2;
extern void * shougatu_cal_cw_res_4;

void shougatu_ptr_set(void)
{
    shougatu_cal_cw_res = (void *)0x801A6558;
    shougatu_cal_cw_res_1 = (void *)0x801A655C;
    shougatu_cal_cw_res_2 = (void *)0x801A6580;
    shougatu_cal_cw_res_4 = (void *)0x801A0000;
}
