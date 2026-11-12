/* Function: date_buf_tbl2_init */
/* Overlay: DATE */
/* Address: 0x801323B4 */
/* Size: 160 bytes (40 instructions) */
#include "types.h"

extern s32 date_spring_res;
extern s32 date_spring_res_hdr1;
extern s32 date_spring_res_img0;
extern s32 date_spring_res_img1;
extern s32 date_spring_res_anim_tbl0;
extern s32 date_spring_res_anim_tbl1;
extern s16 date_spring_res_aux0;
extern s16 date_spring_res_aux1;
extern s32 date_spring_res_pal0;
extern s32 date_spring_res_pal1;

void date_buf_tbl2_init(void) {
    date_spring_res = (s32)0x801F0638;
    date_spring_res_hdr1 = (s32)0x801F0C34;
    date_spring_res_img0 = (s32)0x801F063C;
    date_spring_res_img1 = (s32)0x801F0C3C;
    date_spring_res_anim_tbl0 = (s32)0x801F071C;
    date_spring_res_anim_tbl1 = (s32)0x801F0CEC;
    date_spring_res_aux0 = (*(s16 *)0x801F0D14);
    date_spring_res_aux1 = (*(s16 *)0x801F0D18);
    date_spring_res_pal0 = (s32)0x801E0000;
    date_spring_res_pal1 = (s32)0x801E2000;
}
