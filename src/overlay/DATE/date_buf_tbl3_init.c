/* Function: date_buf_tbl3_init */
/* Overlay: DATE */
/* Address: 0x80132454 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern s32 date_aug_res;
extern s32 date_aug_res_img;
extern s32 date_aug_res_anim_tbl;
extern s16 date_aug_res_aux;
extern s32 date_aug_res_pal;

void date_buf_tbl3_init(void) {
    date_aug_res = (s32)0x801BE078;
    date_aug_res_img = (s32)0x801BE084;
    date_aug_res_anim_tbl = (s32)0x801BE088;
    date_aug_res_aux = (*(s16 *)0x801BE090);
    date_aug_res_pal = (s32)0x801B0000;
}
