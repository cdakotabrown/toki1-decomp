/* Address: 0x80137850 */
/* Size: 64 bytes (16 instructions) */
/* @SymbolSize: 64 */
#include "types.h"

extern void * shugaku_cw96_res;
extern void * shugaku_cw96_res_img;
extern void * shugaku_cw96_res_anim_tbl;
extern void * shugaku_cw96_res_pal;

void shugaku_ptr_set(void)
{
    shugaku_cw96_res = (void *)0x801D60E0;
    shugaku_cw96_res_img = (void *)0x801D60E4;
    shugaku_cw96_res_anim_tbl = (void *)0x801D60F4;
    shugaku_cw96_res_pal = (void *)0x801D0000;
}
