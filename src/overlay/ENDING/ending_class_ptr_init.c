/* @SymbolSize: 780 */
/* ending_class_ptr_init - ENDING overlay, Address: 0x80133C20, Size: 780 bytes (195 insns) */
#include "types.h"

extern s32 ending_cw_hdr_tbl[];
extern s32 ending_cw_img_tbl[];
extern s32 ending_cw_anim_tbl[];
extern s32 ending_bg_res;
extern s32 ending_bg_res_bg0;
extern s32 ending_bg_res_bg1;
extern s32 ending_bg_res_bg2;
extern s32 ending_bg_res_bg3;
extern s32 ending_bg_res_bg4;
extern s32 ending_bg_res_bg5;

void ending_class_ptr_init(void)
{
    ending_cw_hdr_tbl[0] = 0x801B6550;
    ending_cw_hdr_tbl[1] = 0x801B6C6C;
    ending_cw_hdr_tbl[2] = 0x801B7364;
    ending_cw_hdr_tbl[3] = 0x801B7AB0;
    ending_cw_hdr_tbl[4] = 0x801B8200;
    ending_cw_hdr_tbl[5] = 0x801B8834;
    ending_cw_hdr_tbl[6] = 0x801B8EC4;
    ending_cw_hdr_tbl[7] = 0x801B9520;
    ending_cw_hdr_tbl[8] = 0x801B9B98;
    ending_cw_hdr_tbl[9] = 0x801BA088;
    ending_cw_hdr_tbl[10] = 0x801BA790;
    ending_cw_hdr_tbl[11] = 0x801BACB4;
    ending_cw_hdr_tbl[12] = 0x801BB3B0;
    ending_cw_hdr_tbl[13] = 0x801BB760;
    ending_cw_img_tbl[0] = 0x801B6554;
    ending_cw_img_tbl[1] = 0x801B6C70;
    ending_cw_img_tbl[2] = 0x801B7368;
    ending_cw_img_tbl[3] = 0x801B7AB4;
    ending_cw_img_tbl[4] = 0x801B8204;
    ending_cw_img_tbl[5] = 0x801B8838;
    ending_cw_img_tbl[6] = 0x801B8EC8;
    ending_cw_img_tbl[7] = 0x801B9524;
    ending_cw_img_tbl[8] = 0x801B9B9C;
    ending_cw_img_tbl[9] = 0x801BA08C;
    ending_cw_img_tbl[10] = 0x801BA794;
    ending_cw_img_tbl[11] = 0x801BACB8;
    ending_cw_img_tbl[12] = 0x801BB3B4;
    ending_cw_img_tbl[13] = 0x801BB764;
    ending_cw_anim_tbl[0] = 0x801B65AC;
    ending_cw_anim_tbl[1] = 0x801B6D14;
    ending_cw_anim_tbl[2] = 0x801B7408;
    ending_cw_anim_tbl[3] = 0x801B7B5C;
    ending_cw_anim_tbl[4] = 0x801B82AC;
    ending_cw_anim_tbl[5] = 0x801B88C4;
    ending_cw_anim_tbl[6] = 0x801B8F60;
    ending_cw_anim_tbl[7] = 0x801B95B4;
    ending_cw_anim_tbl[8] = 0x801B9C30;
    ending_cw_anim_tbl[9] = 0x801BA128;
    ending_cw_anim_tbl[10] = 0x801BA834;
    ending_cw_anim_tbl[11] = 0x801BAD58;
    ending_cw_anim_tbl[12] = 0x801BB454;
    ending_cw_anim_tbl[13] = 0x801BB7CC;
    ending_bg_res = 0x80180000;
    ending_bg_res_bg0 = 0x80184000;
    ending_bg_res_bg1 = 0x80188000;
    ending_bg_res_bg2 = 0x8018C000;
    ending_bg_res_bg3 = 0x80190000;
    ending_bg_res_bg4 = 0x80194000;
    ending_bg_res_bg5 = 0x80198000;
}
