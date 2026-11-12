/* Address: 0x80132940 */
/* Size: 1144 bytes (286 insns) */
/* @SymbolSize: 1144 */
/* @RodataBase: 0x80139FB0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void load_tpage_buf_lock(s32, s32, s32);

extern s32 bunka_sd_rollcall_res_3;
extern s32 bunka_sd_rollcall_res_1;
extern s32 bunka_sd_rollcall_res_2;
extern s32 *bunka_sd_rollcall_res;
extern s32 bunka_sd_kind;

extern u8  cal_cw_prio;
extern s32 cal_cw_attr;
extern u8  cal_cw_disp;
extern u8  cal_cw_mode;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s16 cal_cw_frame;
extern u8  cal_cw_bright;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern u8  cal_cw_sel;
extern u8  cal_cw_clut;
extern u8  cal_cw_flag4;

extern u8  cw97_prio;
extern s32 cw97_attr;
extern u8  cw97_disp;
extern u8  cw97_mode;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s32 cw97_hdr;
extern s16 cw97_frame;
extern u8  cw97_bright;
extern s16 cw97_x;
extern s16 cw97_y;
extern u8  cw97_sel;
extern u8  cw97_clut;

extern CLASS_W class_w[];

void rollcall_init(void) {
    s32 i;
    u8 *p;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_rollcall_res_3, (u8)(i + 16), 1, 1, i);
    }

    class_clear_n(96);
    cal_cw_prio = 5;
    cal_cw_attr = 0x01000000;
    cal_cw_disp = 132;
    cal_cw_mode = 64;
    cal_cw_img = bunka_sd_rollcall_res_1;
    cal_cw_anim_tbl = bunka_sd_rollcall_res_2;
    cal_cw_hdr = (s32)bunka_sd_rollcall_res;
    cal_cw_frame = 196;
    cal_cw_bright = 0;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_sel = 128;
    cal_cw_clut = 0;
    cal_cw_flag4 = 0;

    class_clear_n(97);
    cw97_prio = 4;
    cw97_attr = 0x01000000;
    cw97_disp = 132;
    cw97_mode = 64;
    cw97_img = bunka_sd_rollcall_res_1;
    cw97_anim_tbl = bunka_sd_rollcall_res_2;
    cw97_hdr = (s32)bunka_sd_rollcall_res;
    cw97_frame = 0;
    cw97_bright = 0;
    cw97_x = -160;
    cw97_y = -120;
    cw97_sel = 128;
    cw97_clut = 0;
    class_w[97].cel_mode = 0;

    p = (void *)(&class_w[2].active);
    for (i = 2; i != 4; i++) {
        class_clear_n(i + 96);
        p[6529] = 4;
        *(s32 *)(p + 6584) = 0x01000000;
        p[6531] = 4;
        *(s32 *)(p + 6540) = bunka_sd_rollcall_res_1;
        *(s32 *)(p + 6544) = bunka_sd_rollcall_res_2;
        *(s32 *)(p + 6580) = (s32)bunka_sd_rollcall_res;
        p += 68;
        *(s16 *)(p + 6482) = bunka_sd_kind * 2 + i - 2;
        p[6466] = 1;
        p[6467] = 0;
        *(s16 *)(p + 6498) = -160;
        *(s16 *)(p + 6502) = -120;
        p[6465] = 128;
        p[6527] = 0;
        p[6464] = 0;
    }

    class_clear_n(100);
    class_w[100].prio = 4;
    (*(s32 *)&class_w[100].attribute) = 0x01000000;
    class_w[100].disp_flag = 132;
    class_w[100].anim_flag = 64;
    (*(s32 *)&class_w[100].cels) = bunka_sd_rollcall_res_1;
    (*(s32 *)&class_w[100].anim_tbl) = bunka_sd_rollcall_res_2;
    (*(s32 *)&class_w[100].hdr) = (s32)bunka_sd_rollcall_res;
    switch (bunka_sd_kind) {
    case 0:
        class_w[100].frame = 191;
        break;
    case 1:
        class_w[100].frame = 194;
        break;
    case 2:
        class_w[100].frame = 192;
        break;
    case 3:
        class_w[100].frame = 193;
        break;
    case 4:
    default:
        class_w[100].frame = 195;
        break;
    }
    class_w[100].bright = 0;
    class_w[100].x.half[1] = -160;
    class_w[100].y.half[1] = -120;
    class_w[100].tpage_sel = 128;
    class_w[100].clut = 0;
    class_w[100].cel_mode = 0;

    load_tpage_buf_lock(5, bunka_sd_rollcall_res[0], 1);
    load_tpage_buf_lock(6, bunka_sd_rollcall_res[1], 1);
    load_tpage_buf_lock(7, bunka_sd_rollcall_res[2], 1);
    load_tpage_buf_lock(8, bunka_sd_rollcall_res[3], 1);
    load_tpage_buf_lock(9, bunka_sd_rollcall_res[4], 1);
    load_tpage_buf_lock(10, bunka_sd_rollcall_res[5], 1);
    load_tpage_buf_lock(37, bunka_sd_rollcall_res[6], 1);
    load_tpage_buf_lock(38, bunka_sd_rollcall_res[7], 1);
    load_tpage_buf_lock(39, bunka_sd_rollcall_res[8], 1);
}
