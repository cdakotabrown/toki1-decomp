/* Size: 640 bytes */
/* @SymbolSize: 640 */
#include "types.h"

extern s32 taiiku_spr_img;
extern s32 taiiku_spr_img_1;
extern s32 taiiku_spr_img_2;
extern s32 taiiku_spr_img_3;
extern s32 taiiku_spr_img_4;
extern s32 taiiku_spr_img_5;
extern s32 taiiku_spr_img_6;
extern s32 taiiku_spr_img_7;
extern s32 taiiku_spr_img_8;
extern s32 taiiku_spr_img_9;
extern s32 taiiku_spr_img_10;
extern s32 taiiku_spr_img_11;
extern s32 taiiku_spr_img_12;
extern s32 taiiku_spr_img_13;
extern s32 taiiku_spr_anim;
extern s32 taiiku_spr_anim_1;
extern s32 taiiku_spr_anim_2;
extern s32 taiiku_spr_anim_3;
extern s32 taiiku_spr_anim_4;
extern s32 taiiku_spr_anim_5;
extern s32 taiiku_spr_anim_6;
extern s32 taiiku_spr_anim_7;
extern s32 taiiku_spr_anim_8;
extern s32 taiiku_spr_anim_9;
extern s32 taiiku_spr_anim_10;
extern s32 taiiku_spr_anim_11;
extern s32 taiiku_spr_anim_12;
extern s32 taiiku_spr_anim_13;
extern s32 taiiku_spr_aux[];

void taiiku_tbl_init(void) {
    taiiku_spr_img = 0x80180144;
    taiiku_spr_img_1 = 0x801820FC;
    taiiku_spr_img_2 = 0x80183E7C;
    taiiku_spr_img_3 = 0x80185F94;
    taiiku_spr_img_4 = 0x80186674;
    taiiku_spr_img_5 = 0x80187088;
    taiiku_spr_img_6 = 0x80187F20;
    taiiku_spr_img_7 = 0x80188BC8;
    taiiku_spr_img_8 = 0x80188DA4;
    taiiku_spr_img_9 = 0x80189A90;
    taiiku_spr_img_10 = 0x8018A794;
    taiiku_spr_img_11 = 0x8018C0C8;
    taiiku_spr_img_12 = 0x8018CEE0;
    taiiku_spr_img_13 = 0x8018EA78;
    taiiku_spr_anim = 0x8018016C;
    taiiku_spr_anim_1 = 0x801823BC;
    taiiku_spr_anim_2 = 0x80184100;
    taiiku_spr_anim_3 = 0x801861D4;
    taiiku_spr_anim_4 = 0x801866BC;
    taiiku_spr_anim_5 = 0x80187170;
    taiiku_spr_anim_6 = 0x80188048;
    taiiku_spr_anim_7 = 0x80188C98;
    taiiku_spr_anim_8 = 0x80188DB4;
    taiiku_spr_anim_9 = 0x80189BF4;
    taiiku_spr_anim_10 = 0x8018A89C;
    taiiku_spr_anim_11 = 0x8018C31C;
    taiiku_spr_anim_12 = 0x8018CFEC;
    taiiku_spr_anim_13 = 0x8018EC14;
    (*(s16 *)&taiiku_spr_aux[0]) = *(s16 *)0x8018EC40;
    (*(s16 *)&taiiku_spr_aux[1]) = *(s16 *)0x8018EC44;
    (*(s16 *)&taiiku_spr_aux[2]) = *(s16 *)0x8018EC48;
    (*(s16 *)&taiiku_spr_aux[3]) = *(s16 *)0x8018EC4C;
    (*(s16 *)&taiiku_spr_aux[4]) = *(s16 *)0x8018EC50;
    (*(s16 *)&taiiku_spr_aux[5]) = *(s16 *)0x8018EC54;
    (*(s16 *)&taiiku_spr_aux[6]) = *(s16 *)0x8018EC58;
    (*(s16 *)&taiiku_spr_aux[7]) = *(s16 *)0x8018EC5C;
    (*(s16 *)&taiiku_spr_aux[8]) = *(s16 *)0x8018EC60;
    (*(s16 *)&taiiku_spr_aux[9]) = *(s16 *)0x8018EC64;
    (*(s16 *)&taiiku_spr_aux[10]) = *(s16 *)0x8018EC68;
    (*(s16 *)&taiiku_spr_aux[11]) = *(s16 *)0x8018EC6C;
    (*(s16 *)&taiiku_spr_aux[12]) = *(s16 *)0x8018EC70;
    (*(s16 *)&taiiku_spr_aux[13]) = *(s16 *)0x8018EC74;
}
