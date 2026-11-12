/* @RodataBase: 0x8013C000 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=39:t5 IDO52_COPYPROP_SITE=lod:4:deny,lod:5:deny IDO52_AS1_NO_COPY_PROP=3 IDO52_AS1_NO_PASS1_PROP=443,445 */
/* ending_screen_init - ENDING overlay, Address: 0x80134280, */
/* Size: 1848 bytes (462 instructions) */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x228 */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x228 */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x228 */
/* @PostProc: swap_instructions --offset1 0x22c --offset2 0x230 */
/* @PostProc: swap_instructions --offset1 0x238 --offset2 0x23c */
/* @PostProc: swap_instructions --offset1 0x240 --offset2 0x244 */
/* @PostProc: swap_instructions --offset1 0x358 --offset2 0x368 */
/* @PostProc: swap_instructions --offset1 0x360 --offset2 0x368 */
#include "types.h"

typedef struct {
    u32 f0;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    u8  f14;
    u8  f15;
    s16 f16;
    s16 f18;
    u8  f20;
    u8  f21;
    u8  f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f30;
    u32 f32;
} EndRec;

typedef struct {
    s32 w[17];
} EndState;

extern EndRec Sprites[];
extern u8 screen_w[];

extern void SetDispMask();
extern void ending_class_ptr_init();
extern void palette_upload_enqueue();
extern void load_image_rect();
extern void class_clear_n();
extern void sub_sub_step_up();

extern s32 ending_bg_res;
extern s32 ending_bg_res_bg0;
extern s32 ending_bg_res_bg1;
extern s32 ending_bg_res_bg2;
extern s32 ending_bg_res_bg3;
extern s32 ending_bg_res_bg4;
extern s32 ending_bg_res_bg5;

extern s32 etc_rec_tbl0;
extern s32 etc_rec_tbl1;
extern s32 etc_rec_tbl2;
extern s32 etc_rec_tbl3;
extern s32 etc_rec_tbl4;
extern s32 etc_rec_tbl5;

extern EndState cw96_active;
extern EndState cw97_active;

extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_flag4;
extern u8  cal_cw_sel;
extern u8  cal_cw_rate;
extern u8  cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8  cal_cw_clut;
extern u8  cw97_disp;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s16 cw97_w14;
extern s16 cw97_x;
extern s32 cw97_hdr;

extern s32 ending_cw_hdr_tbl[];
extern s32 ending_cw_img_tbl[];
extern s32 ending_cw_anim_tbl[];
extern s32 ending_cw_aux_tbl[];


extern u8 now_girl;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;

void ending_screen_init(void)
{
    s16 i;
    u8 step;
    s32 lum;
    s32 zv;

    lum = 128;

    SetDispMask(1);
    ending_class_ptr_init();

    palette_upload_enqueue(ending_bg_res, 16, 1, 2, 0);
    palette_upload_enqueue(ending_bg_res, 17, 1, 2, 1);

    load_image_rect(320, 128, 64, 128, ending_bg_res_bg0);
    load_image_rect(384, 128, 64, 128, ending_bg_res_bg1);
    load_image_rect(448, 128, 64, 128, ending_bg_res_bg2);
    load_image_rect(512, 128, 64, 128, ending_bg_res_bg3);
    load_image_rect(576, 128, 64, 128, ending_bg_res_bg4);
    load_image_rect(640, 128, 64, 128, ending_bg_res_bg5);

    etc_rec_tbl0 = 1;
    etc_rec_tbl1 = 1;
    etc_rec_tbl2 = 1;
    etc_rec_tbl3 = 1;
    etc_rec_tbl4 = 1;
    etc_rec_tbl5 = 1;

    for (i = 0; i < 6; i++) {
        Sprites[i + 4].f12 = i + 5;
        Sprites[i + 4].f0 = 0x41000000;
        Sprites[i + 4].f8 = lum;
        Sprites[i + 4].f10 = lum;
        Sprites[i + 4].f14 = 0;
        Sprites[i + 4].f15 = lum;
        Sprites[i + 4].f16 = 0;
        Sprites[i + 4].f18 = 496;
        Sprites[i + 4].f20 = 0;
        Sprites[i + 4].f21 = 0;
        Sprites[i + 4].f22 = 0;
        Sprites[i + 4].f24 = 0;
        Sprites[i + 4].f26 = 0;
        Sprites[i + 4].f28 = 4096;
        Sprites[i + 4].f30 = 4096;
        Sprites[i + 4].f32 = 0;
        Sprites[i + 4].f4 = (i % 3) * 128 - 128;
        Sprites[i + 4].f6 = (i / 3) * 128 - 120;
        screen_w[i + 4265] = 11;
    }

    class_clear_n(96);

    cal_cw_prio = 12;
    cal_cw_mode = 0;
    cal_cw_attr = 0;
    cal_cw_disp = 132;
    cal_cw_img = ending_cw_img_tbl[0];
    cal_cw_anim_tbl = ending_cw_anim_tbl[0];
    cal_cw_hdr = ending_cw_hdr_tbl[0];
    cal_cw_aux14 = (*(s16 *)&ending_cw_aux_tbl[0]);
    cal_cw_anim = 0;
    cal_cw_frame = 0;
    cal_cw_rate = 1;
    cal_cw_clut = 17;
    cal_cw_x = -32;
    cal_cw_y = 40;
    cal_cw_bright = 0;
    cal_cw_sel = 1;
    cal_cw_flag4 = 0;

    cw97_active = cw96_active;

    cw97_disp = 4;
    cw97_x = 16;

    step = now_girl;
    switch (step) {
    case 0:
        cw97_img = ending_cw_img_tbl[1];
        cw97_anim_tbl = ending_cw_anim_tbl[1];
        cw97_hdr = ending_cw_hdr_tbl[1];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[1]);
        break;
    case 1:
        cw97_img = ending_cw_img_tbl[2];
        cw97_anim_tbl = ending_cw_anim_tbl[2];
        cw97_hdr = ending_cw_hdr_tbl[2];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[2]);
        break;
    case 2:
        cw97_img = ending_cw_img_tbl[3];
        cw97_anim_tbl = ending_cw_anim_tbl[3];
        cw97_hdr = ending_cw_hdr_tbl[3];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[3]);
        break;
    case 3:
        cw97_img = ending_cw_img_tbl[4];
        cw97_anim_tbl = ending_cw_anim_tbl[4];
        cw97_hdr = ending_cw_hdr_tbl[4];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[4]);
        break;
    case 4:
        cw97_img = ending_cw_img_tbl[5];
        cw97_anim_tbl = ending_cw_anim_tbl[5];
        cw97_hdr = ending_cw_hdr_tbl[5];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[5]);
        break;
    case 5:
        cw97_img = ending_cw_img_tbl[6];
        cw97_anim_tbl = ending_cw_anim_tbl[6];
        cw97_hdr = ending_cw_hdr_tbl[6];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[6]);
        break;
    case 6:
        cw97_img = ending_cw_img_tbl[7];
        cw97_anim_tbl = ending_cw_anim_tbl[7];
        cw97_hdr = ending_cw_hdr_tbl[7];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[7]);
        break;
    case 7:
        cw97_img = ending_cw_img_tbl[8];
        cw97_anim_tbl = ending_cw_anim_tbl[8];
        cw97_hdr = ending_cw_hdr_tbl[8];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[8]);
        break;
    case 8:
        cw97_img = ending_cw_img_tbl[9];
        cw97_anim_tbl = ending_cw_anim_tbl[9];
        cw97_hdr = ending_cw_hdr_tbl[9];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[9]);
        break;
    case 9:
        cw97_img = ending_cw_img_tbl[10];
        cw97_anim_tbl = ending_cw_anim_tbl[10];
        cw97_hdr = ending_cw_hdr_tbl[10];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[10]);
        break;
    case 10:
        cw97_img = ending_cw_img_tbl[11];
        cw97_anim_tbl = ending_cw_anim_tbl[11];
        cw97_hdr = ending_cw_hdr_tbl[11];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[11]);
        break;
    case 12:
        cw97_img = ending_cw_img_tbl[12];
        cw97_anim_tbl = ending_cw_anim_tbl[12];
        cw97_hdr = ending_cw_hdr_tbl[12];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[12]);
        break;
    default:
        cw97_img = ending_cw_img_tbl[13];
        cw97_anim_tbl = ending_cw_anim_tbl[13];
        cw97_hdr = ending_cw_hdr_tbl[13];
        cw97_w14 = (*(s16 *)&ending_cw_aux_tbl[13]);
        break;
    }

    zv = 0;
    back_clear_g = zv;
    back_clear_r = zv;
    back_clear_b = 0;
    sub_sub_step_up();
}
