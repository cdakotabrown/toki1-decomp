/* Address: 0x801362E0 */
/* Size: 180 bytes (45 instructions) */
/* @SymbolSize: 180 */
#include "types.h"

extern void * etc_bustup_buf_res;
extern void * etc_bustup_buf_res_img;
extern void * etc_bustup_buf_res_anim_tbl;
extern s16 etc_bustup_buf_res_aux;
extern void * etc_bustup_buf_res_buf0;
extern void * etc_bustup_buf_res_buf1;
extern void * etc_bustup_buf_res_buf2;
extern void * etc_bustup_buf_res_buf3;
extern void * etc_bustup_buf_res_buf4;
extern void * etc_bustup_buf_res_buf5;
extern void * etc_bustup_buf_res_buf6;

void bustup_buf_ptrs_init(void)
{
    etc_bustup_buf_res = (void *)0x801B93F0;
    etc_bustup_buf_res_img = (void *)0x801B9360;
    etc_bustup_buf_res_anim_tbl = (void *)0x801B93D8;
    etc_bustup_buf_res_aux = *(s16 *)0x801B93F8;
    etc_bustup_buf_res_buf0 = (void *)0x80197000;
    etc_bustup_buf_res_buf1 = (void *)0x80199000;
    etc_bustup_buf_res_buf2 = (void *)0x8019D000;
    etc_bustup_buf_res_buf3 = (void *)0x801A1000;
    etc_bustup_buf_res_buf4 = (void *)0x801A5000;
    etc_bustup_buf_res_buf5 = (void *)0x801A9000;
    etc_bustup_buf_res_buf6 = (void *)0x801AD000;
}
