/* @RodataBase: 0x8015C170 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 class_clear_n(s32);
extern s32 load_tpage_buf_lock(s32, s32, s32);
extern s32 vram_area_clear(s32);
extern s32 rpg_bat_slot_anim_set(s32, s32, s32);

extern s32 *rpg_bat_res_hdr;
extern s32 *rpg_bat_res_hdr_1;
extern s32 *rpg_bat_res_hdr_2;
extern s32 *rpg_bat_res_hdr_3;
extern s32 *rpg_bat_res_hdr_4;
extern s32 *rpg_bat_res_hdr_5;

extern s32 rpg_bat_res_img[];
extern s32 rpg_bat_res_anim[];

extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_bukatu_no;
extern s32 rpg_bat_bukatu_flag;

extern u8 cw56_rec[];

typedef struct {
    u8 f0; u8 f1; u8 f2; u8 f3; u8 f4; u8 f5; u8 f6; u8 f7;
    s32 f8; s32 f12; s32 f16; u8 pad20[18]; s16 f38; s16 pad40; s16 f42;
    u8 pad44[8]; s32 f52; s32 f56; u8 pad60[7]; u8 f67;
} Rec141060;
extern Rec141060 class_w[] ;
void rpg_bat_rec_init(void)
{
    s32 i;
    s32 s0;
    s32 s1;
    s32 sel;

    for (s1 = 0; s1 < 96; s1++) {
        class_clear_n(s1 + 64);
        if (s1 < 63) {
            class_w[s1 + 64].f1 = 3;
        } else if (s1 < 64) {
            class_w[s1 + 64].f1 = 9;
        } else if (s1 < 72) {
            class_w[s1 + 64].f1 = 4;
        } else {
            class_w[s1 + 64].f1 = 5;
        }
        class_w[s1 + 64].f56 = 0x41000000;
        class_w[s1 + 64].f3 = 0;
        class_w[s1 + 64].f2 = 1;
        class_w[s1 + 64].f6 = 1;
        class_w[s1 + 64].f7 = 0;
        class_w[s1 + 64].f38 = -160;
        class_w[s1 + 64].f42 = -120;
        class_w[s1 + 64].f5 = 128;
        class_w[s1 + 64].f67 = 0;
        class_w[s1 + 64].f4 = 8;
        class_w[s1 + 64].f12 = rpg_bat_res_img[0];
        class_w[s1 + 64].f16 = rpg_bat_res_anim[0];
        class_w[s1 + 64].f52 = (s32)rpg_bat_res_hdr;
    }

    for (s1 = 0; s1 < 72; s1++) {
        class_w[s1 + 64].f12 = rpg_bat_res_img[0];
        class_w[s1 + 64].f16 = rpg_bat_res_anim[0];
        class_w[s1 + 64].f52 = (s32)rpg_bat_res_hdr;
    }

    load_tpage_buf_lock(21, rpg_bat_res_hdr[0], 1);
    load_tpage_buf_lock(22, rpg_bat_res_hdr[1], 1);
    load_tpage_buf_lock(23, rpg_bat_res_hdr[2], 1);
    load_tpage_buf_lock(24, rpg_bat_res_hdr[3], 1);
    load_tpage_buf_lock(25, rpg_bat_res_hdr[4], 1);
    load_tpage_buf_lock(53, rpg_bat_res_hdr_3[0], 1);
    load_tpage_buf_lock(57, rpg_bat_res_hdr_3[1], 1);

    if (rpg_bat_mode == 0x80000000) goto Lc80;
    if (rpg_bat_mode == 0x00010000) goto Lc010000;
    if (rpg_bat_mode == 0x00020000) goto L141;
    if (rpg_bat_mode == 0x00040000) goto L141;
    if (rpg_bat_mode == 0x00100000) goto Lc100000;
    if (rpg_bat_mode == 0x08000000) goto L300;
    s1 = 0;
    goto L394;

Lc010000:
    load_tpage_buf_lock(11, rpg_bat_res_hdr_2[1], 1);
    load_tpage_buf_lock(12, rpg_bat_res_hdr_2[2], 1);

L141:
    load_tpage_buf_lock(13, rpg_bat_res_hdr_2[0], 1);
    load_tpage_buf_lock(43, rpg_bat_res_hdr_1[0], 1);
    load_tpage_buf_lock(44, rpg_bat_res_hdr_1[1], 1);
    load_tpage_buf_lock(45, rpg_bat_res_hdr_1[2], 1);
    if (rpg_bat_girl == 13) {
        load_tpage_buf_lock(5, rpg_bat_res_hdr_5[0], 1);
        load_tpage_buf_lock(6, rpg_bat_res_hdr_5[1], 1);
        load_tpage_buf_lock(7, rpg_bat_res_hdr_5[2], 1);
        s1 = 0;
        goto L394;
    }
    goto L393;

Lc80:
    load_tpage_buf_lock(11, rpg_bat_res_hdr_5[0], 1);
    load_tpage_buf_lock(12, rpg_bat_res_hdr_5[1], 1);
    load_tpage_buf_lock(13, rpg_bat_res_hdr_5[2], 1);
    load_tpage_buf_lock(14, rpg_bat_res_hdr_5[3], 1);
    load_tpage_buf_lock(15, rpg_bat_res_hdr_5[4], 1);
    load_tpage_buf_lock(43, rpg_bat_res_hdr_5[5], 1);
    load_tpage_buf_lock(45, rpg_bat_res_hdr_1[0], 1);
    load_tpage_buf_lock(46, rpg_bat_res_hdr_1[1], 1);
    load_tpage_buf_lock(47, rpg_bat_res_hdr_1[2], 1);
    s1 = 0;
    goto L394;

Lc100000:
    load_tpage_buf_lock(11, rpg_bat_res_hdr_1[6], 1);
    if (rpg_bat_girl == 14) goto L300;
    switch (rpg_bat_girl) {
    case 0:  s1 = 1; break;
    case 1:  s1 = 2; break;
    case 2:  s1 = 4; break;
    case 3:  s1 = 3; break;
    case 4:  s1 = 2; break;
    case 5:  s1 = 2; break;
    case 6:  s1 = 3; break;
    case 7:  s1 = 2; break;
    case 8:  s1 = 2; break;
    case 9:  s1 = 1; break;
    case 10: s1 = 2; break;
    case 13: s1 = 3; break;
    }
    for (s0 = 0; s0 < s1; s0++) {
        load_tpage_buf_lock(s0 + 37, rpg_bat_res_hdr_5[s0], 1);
    }

L300:
    if (rpg_bat_bukatu_flag != 1) goto L357;
    s0 = 0;
    switch (rpg_bat_bukatu_no) {
    case 0:  s1 = 4; break;
    case 1:  s1 = 4; break;
    case 2:  s1 = 4; break;
    case 3:  s1 = 5; break;
    case 4:  s1 = 5; break;
    case 5:  s1 = 4; break;
    case 6:  s1 = 3; break;
    case 7:  s1 = 3; break;
    case 8:  s1 = 5; break;
    case 9:  s1 = 5; break;
    case 10: s1 = 3; break;
    case 11: s1 = 6; break;
    }

    do {
        if (s0 < s1) {
            load_tpage_buf_lock(s0 + 5, rpg_bat_res_hdr_4[s0], 1);
        } else {
            vram_area_clear(s0 + 5);
        }
        s0++;
    } while (s0 != 6);

L357:
    load_tpage_buf_lock(44, rpg_bat_res_hdr_1[0], 1);
    load_tpage_buf_lock(12, rpg_bat_res_hdr_1[1], 1);
    load_tpage_buf_lock(13, rpg_bat_res_hdr_1[2], 1);
    load_tpage_buf_lock(14, rpg_bat_res_hdr_1[3], 1);
    load_tpage_buf_lock(15, rpg_bat_res_hdr_1[4], 1);
    load_tpage_buf_lock(43, rpg_bat_res_hdr_1[5], 1);
L393:
    s1 = 0;

L394:
    do {
        rpg_bat_slot_anim_set(s1, 39, 0);
        s1++;
    } while (s1 < 48);

    if (rpg_bat_girl != 14) {
        for (i = 29; i < 31; i++) {
            class_w[i + 96].f12 = rpg_bat_res_img[5];
            class_w[i + 96].f16 = rpg_bat_res_anim[5];
            class_w[i + 96].f52 = (s32)rpg_bat_res_hdr_5;
            class_w[i + 96].f67 = 28;
            class_w[i + 96].f1 = 5;
        }
    }

    if (rpg_bat_bukatu_flag == 1) {
        for (i = 40; i < 48; i++) {
            class_w[i + 96].f12 = rpg_bat_res_img[4];
            class_w[i + 96].f16 = rpg_bat_res_anim[4];
            class_w[i + 96].f52 = (s32)rpg_bat_res_hdr_4;
            class_w[i + 96].f67 = 29;
        }
    }

    sel = rpg_bat_mode;
    if ((sel & 0x000F0000) != 0) {
        for (i = 40; i < 48; i++) {
            class_w[i + 96].f12 = rpg_bat_res_img[2];
            class_w[i + 96].f16 = rpg_bat_res_anim[2];
            class_w[i + 96].f52 = (s32)rpg_bat_res_hdr_2;
            class_w[i + 96].f67 = 24;
        }
    }

    if (sel & 0x80000000) {
        for (i = 40; i < 48; i++) {
            class_w[i + 96].f12 = rpg_bat_res_img[1];
            class_w[i + 96].f16 = rpg_bat_res_anim[1];
            class_w[i + 96].f52 = (s32)rpg_bat_res_hdr_1;
            class_w[i + 96].f67 = 18;
        }
    }

    for (s1 = 48; s1 < 50; s1++) {
        class_w[s1 + 96].f12 = rpg_bat_res_img[3];
        class_w[s1 + 96].f16 = rpg_bat_res_anim[3];
        class_w[s1 + 96].f52 = (s32)rpg_bat_res_hdr_3;
        class_w[s1 + 96].f67 = 30;
    }

    if (rpg_bat_bukatu_flag == 1) {
        for (s1 = 50; s1 < 56; s1++) {
            class_w[s1 + 96].f12 = rpg_bat_res_img[4];
            class_w[s1 + 96].f16 = rpg_bat_res_anim[4];
            class_w[s1 + 96].f52 = (s32)rpg_bat_res_hdr_4;
            class_w[s1 + 96].f67 = 29;
        }
    }

    for (i = 56; i < 64; i++) {
        class_w[i + 96].f12 = rpg_bat_res_img[1];
        class_w[i + 96].f16 = rpg_bat_res_anim[1];
        class_w[i + 96].f52 = (s32)rpg_bat_res_hdr_1;
        if (sel == 0x00020000) {
            class_w[i + 96].f67 = 19;
        } else {
            class_w[i + 96].f67 = 18;
        }
    }
}
