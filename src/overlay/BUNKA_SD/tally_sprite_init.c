/* Address: 0x80134938 */
/* Size: 592 bytes (148 insns) */
/* @SymbolSize: 592 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void search_load_tpage_buf_lock(s32, s32);

extern s32 bunka_sd_tally_res_3;
extern s32 bunka_sd_tally_res_1;
extern s32 bunka_sd_tally_res_2;
extern s32 *bunka_sd_tally_res;
extern s32 bunka_sd_kind;

extern u8  cal_cw_prio;
extern s32 cal_cw_attr;
extern u8  cal_cw_disp;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s16 cal_cw_anim;
extern u8  cal_cw_bright;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern u8  cal_cw_sel;
extern u8  cal_cw_clut;
extern u8  cal_cw_flag4;
extern CLASS_W class_w[];

void tally_sprite_init(void) {
    s32 i;
    s32 v0;
    u8 *p;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_tally_res_3, (u8)(i + 16), 1, 1, i);
    }

    class_clear_n(96);
    cal_cw_prio = 5;
    cal_cw_attr = 0x01000000;
    cal_cw_disp = 128;
    cal_cw_img = bunka_sd_tally_res_1;
    cal_cw_anim_tbl = bunka_sd_tally_res_2;
    cal_cw_hdr = (s32)bunka_sd_tally_res;
    cal_cw_anim = 10;
    cal_cw_bright = 0;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_sel = 128;
    cal_cw_clut = 0;
    cal_cw_flag4 = 0;

    p = (void *)(&class_w[1].active);
    for (i = 1; i != 5; i++) {
        class_clear_n(i + 96);
        p[6529] = 4;
        *(s32 *)(p + 6584) = 0x01000000;
        p[6531] = 0;
        *(s32 *)(p + 6540) = bunka_sd_tally_res_1;
        *(s32 *)(p + 6544) = bunka_sd_tally_res_2;
        *(s32 *)(p + 6580) = (s32)bunka_sd_tally_res;
        p += 68;
        *(s16 *)(p + 6482) = i - 1;
        p[6466] = 1;
        p[6467] = 0;
        *(s16 *)(p + 6498) = -160;
        *(s16 *)(p + 6502) = -120;
        p[6465] = 128;
        p[6527] = 0;
        p[6464] = 0;
    }

    v0 = bunka_sd_kind;
    class_w[99].anim = v0 * 2 + 2;
    class_w[99].disp_flag = 128;
    class_w[100].anim = v0 * 2 + 3;
    search_load_tpage_buf_lock(bunka_sd_tally_res[11], 1);
    search_load_tpage_buf_lock(bunka_sd_tally_res[12], 1);
    search_load_tpage_buf_lock(bunka_sd_tally_res[13], 1);
    search_load_tpage_buf_lock(bunka_sd_tally_res[14], 1);
}
