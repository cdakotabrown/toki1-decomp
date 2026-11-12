/* @SymbolStart: 0x801505A8 */
/* @SymbolSize: 732 */
/* Address: 0x801505CC  Size: 696 bytes (174 insns; .sym over-sizes) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"
#include "tokimeki/sprite.h"
#include "tokimeki/class_w.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 param_clamp();
extern s32 class_clear_n();
extern s32 load_image_rect();
extern s32 date_girl2_mode4_res_ptr_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s16 cw98_w14;
extern s16 cw98_anim;
extern s16 cw98_frame;
extern s16 cw98_x;
extern s16 cw98_y;
extern volatile u8 sprite_state57;
extern volatile s16 girl2_yuukou;
extern volatile s16 girl2_syoushin;
extern s32 tpage_buf[];
extern CLASS_W class_w[];
extern SPRITE_REC Sprites[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];
extern DATE_OBJ_RES_EX date_ev11_res;

void date_girl2_mode4_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[17];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[17];
    date_sss_soundev = date_soundev_tbl[17];
    date_girl2_mode4_res_ptr_init();
    palette_upload_enqueue(date_ev11_res.pal, 17, 1, 2, 0);
    date_sprite_init(date_ev11_res.tile[0], date_ev11_res.tile[1], date_ev11_res.tile[2], date_ev11_res.tile[3],
                  date_ev11_res.tile[4], date_ev11_res.tile[5]);
    date_object_set(date_ev11_res.img, date_ev11_res.anim_tbl, date_ev11_res.hdr, (s32)date_ev11_res.aux);
    class_clear_n(98);
    class_w[98].prio = 9;
    class_w[98].anim_flag = 1;
    (*(s32 *)&class_w[98].attribute) = 0x41000000;
    class_w[98].disp_flag = 4;
    (*(s32 *)&class_w[98].cels) = date_ev11_res.img;
    (*(s32 *)&class_w[98].anim_tbl) = date_ev11_res.anim_tbl;
    (*(s32 *)&class_w[98].hdr) = date_ev11_res.hdr;
    cw98_w14 = date_ev11_res.aux;
    cw98_anim = 4;
    cw98_frame = 0;
    class_w[98].rate = 1;
    class_w[98].clut = 17;
    cw98_x = -160;
    cw98_y = -120;
    class_w[98].bright = 0;
    class_w[98].tpage_sel = 0;
    class_w[98].cel_mode = 0;
    palette_upload_enqueue(date_ev11_res.pal2, 18, 1, 0, 0);
    tpage_buf[26] = 1;
    load_image_rect(640, 256, 64, 128, date_ev11_res.ext_tile);
    (*(s32 *)&Sprites[57].attribute) = 0x81000000;
    Sprites[57].w = 85;
    Sprites[57].h = 56;
    Sprites[57].u = 0;
    Sprites[57].v = 0;
    Sprites[57].cx = 0;
    Sprites[57].tpage = 26;
    Sprites[57].cy = 498;
    Sprites[57].r = 0;
    Sprites[57].g = 0;
    Sprites[57].b = 0;
    Sprites[57].mx = 0;
    Sprites[57].my = 0;
    Sprites[57].scalex = 4096;
    Sprites[57].scaley = 4096;
    (*(s32 *)&Sprites[57].rotate) = 0;
    Sprites[57].x = 5;
    Sprites[57].y = -30;
    sprite_state57 = 12;
    girl2_yuukou = girl2_yuukou + 2;
    girl2_syoushin = girl2_syoushin - 10;
    param_clamp();
    sub_sub_step_up();
}
