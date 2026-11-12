/* bunka_sd_screen_init - BUNKA_SD overlay, Address: 0x80138D04, Size: 952 bytes (238 insns) */
/* @SymbolSize: 952 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 bunka_sd_screen_res_3;
extern s32 bunka_sd_screen_res_1;
extern s32 bunka_sd_screen_res_2;
extern s32 *bunka_sd_screen_res;
extern s32 bunka_sd_kind;


extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern u8 cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 cal_cw_clut;

extern u8 cw97_prio;
extern u8 cw97_mode;
extern u8 cw97_disp;
extern u8 cw97_sel;
extern u8 cw97_bright;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s16 cw97_frame;
extern s16 cw97_x;
extern s16 cw97_y;
extern s32 cw97_hdr;
extern s32 cw97_attr;
extern u8 cw97_clut;
extern CLASS_W class_w[];


extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void load_tpage_buf_lock(s32, s32, s32);

void bunka_sd_screen_init(void)
{
    s32 i;
    s32 pitch;
    s32 flags;
    u8 *p;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_screen_res_3, (i + 16) & 0xFF, 1, 1, i);
    }

    class_clear_n(96);
    cal_cw_prio = 5;
    cal_cw_attr = 0x01000000;
    cal_cw_disp = 128;
    cal_cw_mode = 64;
    cal_cw_img = bunka_sd_screen_res_1;
    cal_cw_anim_tbl = bunka_sd_screen_res_2;
    cal_cw_hdr = (s32)bunka_sd_screen_res;
    cal_cw_frame = 43;
    cal_cw_bright = 0;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_sel = 128;
    cal_cw_clut = 0;
    cal_cw_flag4 = 0;

    class_clear_n(97);
    cw97_prio = 4;
    cw97_attr = 0x01000000;
    cw97_disp = 128;
    cw97_mode = 64;
    cw97_img = bunka_sd_screen_res_1;
    cw97_anim_tbl = bunka_sd_screen_res_2;
    cw97_hdr = (s32)bunka_sd_screen_res;
    cw97_frame = 0;
    cw97_bright = 0;
    cw97_x = -160;
    cw97_y = -120;
    cw97_sel = 128;
    cw97_clut = 0;
    class_w[97].cel_mode = 0;

    p = (&class_w[2].active);
    i = 2;
    pitch = 15;
    for (; i < 6; i++) {
        class_clear_n(i + 96);
        p[6529] = 4;
        *(s32 *)(p + 6584) = 0x01000000;
        p[6531] = 128;
        p[6530] = 64;
        *(s32 *)(p + 6540) = bunka_sd_screen_res_1;
        *(s32 *)(p + 6544) = bunka_sd_screen_res_2;
        *(s32 *)(p + 6580) = (s32)bunka_sd_screen_res;
        *(s16 *)(p + 6552) = (s16)pitch;
        pitch += 4;
        p += 68;
        p[6466] = 1;
        p[6467] = 0;
        *(s16 *)(p + 6498) = -160;
        *(s16 *)(p + 6502) = -120;
        p[6465] = 128;
        p[6527] = 0;
        p[6464] = 0;
    }

    flags = bunka_sd_kind;
    if (flags & 1) {
        class_w[99].frame = 31;
    }
    if (flags & 2) {
        class_w[100].frame = 35;
    }
    if (flags & 4) {
        class_w[101].frame = 39;
    }

    load_tpage_buf_lock(5, bunka_sd_screen_res[0], 1);
    load_tpage_buf_lock(6, bunka_sd_screen_res[1], 1);
    load_tpage_buf_lock(7, bunka_sd_screen_res[2], 1);
    load_tpage_buf_lock(8, bunka_sd_screen_res[3], 1);
    load_tpage_buf_lock(9, bunka_sd_screen_res[4], 1);
    load_tpage_buf_lock(10, bunka_sd_screen_res[5], 1);
    load_tpage_buf_lock(37, bunka_sd_screen_res[6], 1);
    load_tpage_buf_lock(38, bunka_sd_screen_res[7], 1);
    load_tpage_buf_lock(39, bunka_sd_screen_res[8], 1);
    load_tpage_buf_lock(40, bunka_sd_screen_res[9], 1);
}
