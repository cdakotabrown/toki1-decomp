/* bunka_sd_slot3_init - BUNKA_SD overlay, Address: 0x80135630, Size: 892 bytes (223 insns) */
/* @SymbolSize: 892 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 bunka_sd_slot3_res_3;
extern s32 bunka_sd_slot3_res_1;
extern s32 bunka_sd_slot3_res_2;
extern s32 *bunka_sd_slot3_res;
extern s32 bunka_sd_kind;

extern u8 cal_cw_prio;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u8 cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 cal_cw_clut;

extern u8 cw97_prio;
extern u8 cw97_disp;
extern u8 cw97_sel;
extern u8 cw97_rate;
extern u8 cw97_bright;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s16 cw97_anim;
extern s16 cw97_x;
extern s16 cw97_y;
extern s32 cw97_hdr;
extern s32 cw97_attr;
extern u8 cw97_clut;

extern s16 cw98_frame;
extern s16 cw98_x;
extern s16 cw98_y;
extern CLASS_W class_w[];

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void load_tpage_buf_lock(s32, s32, s32);

void bunka_sd_slot3_init(void)
{
    s32 i;
    s32 *tbl;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(bunka_sd_slot3_res_3, (i + 16) & 0xFF, 1, 1, i);
    }

    class_clear_n(96);
    cal_cw_prio = 4;
    cal_cw_attr = 0x01000000;
    cal_cw_disp = 128;
    cal_cw_img = bunka_sd_slot3_res_1;
    cal_cw_anim_tbl = bunka_sd_slot3_res_2;
    cal_cw_hdr = (s32)bunka_sd_slot3_res;
    cal_cw_anim = (s16)bunka_sd_kind;
    cal_cw_rate = 1;
    cal_cw_bright = 0;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_sel = 128;
    cal_cw_clut = 0;
    cal_cw_flag4 = 0;

    class_clear_n(97);
    cw97_prio = 4;
    cw97_attr = 0x01000000;
    cw97_disp = 0;
    cw97_img = bunka_sd_slot3_res_1;
    cw97_anim_tbl = bunka_sd_slot3_res_2;
    cw97_hdr = (s32)bunka_sd_slot3_res;
    if (bunka_sd_kind == 3) {
        cw97_anim = 5;
    } else {
        cw97_anim = 4;
    }
    cw97_rate = 1;
    cw97_bright = 128;
    cw97_x = -160;
    cw97_y = -120;
    cw97_sel = 0;
    cw97_clut = 17;
    class_w[97].cel_mode = 0;

    class_clear_n(98);
    class_w[98].prio = 4;
    (*(s32 *)&class_w[98].attribute) = 0x01000000;
    class_w[98].disp_flag = 128;
    class_w[98].anim_flag = 64;
    (*(s32 *)&class_w[98].cels) = bunka_sd_slot3_res_1;
    (*(s32 *)&class_w[98].anim_tbl) = bunka_sd_slot3_res_2;
    tbl = bunka_sd_slot3_res;
    (*(s32 *)&class_w[98].hdr) = (s32)tbl;
    cw98_frame = 128;
    class_w[98].rate = 1;
    class_w[98].bright = 0;
    cw98_x = -160;
    cw98_y = -120;
    class_w[98].tpage_sel = 128;
    class_w[98].clut = 0;
    class_w[98].cel_mode = 0;

    load_tpage_buf_lock(5, tbl[0], 1);
    load_tpage_buf_lock(6, bunka_sd_slot3_res[1], 1);
    load_tpage_buf_lock(7, bunka_sd_slot3_res[2], 1);
    load_tpage_buf_lock(8, bunka_sd_slot3_res[3], 1);
    load_tpage_buf_lock(9, bunka_sd_slot3_res[4], 1);
    load_tpage_buf_lock(10, bunka_sd_slot3_res[5], 1);
    load_tpage_buf_lock(37, bunka_sd_slot3_res[6], 1);
    load_tpage_buf_lock(38, bunka_sd_slot3_res[7], 1);
    load_tpage_buf_lock(39, bunka_sd_slot3_res[8], 1);
    load_tpage_buf_lock(40, bunka_sd_slot3_res[9], 1);
}
