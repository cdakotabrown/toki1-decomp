/* @SymbolStart: 0x80135E4C */
/* @SymbolSize: 524 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=8:t3,9:t4,10:t5,16:t6 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x198 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8 b[68];
} Blk68;

extern u8  now_girl;
extern u8  shugaku_float_on;
extern volatile s16 shugaku_float_cnt;
extern u8  menu_x;
extern s16 with_yoshi;
extern s16 move_in;
extern s32 shugaku_ss_serifuev;
extern s32 shugaku_ss_serifuev_kind;
extern s32 shugaku_sss_soundev;
extern s32 shugaku_girl2_res_pal;
extern s32 date_sprite_on;
extern s32 wink2_stop;
extern s32 bustup_mode;
extern u8  Sprites[];
extern s32 shugaku_girl2_res_img;
extern s32 shugaku_girl2_res_anim_tbl;
extern s32 shugaku_girl2_res;
extern s16 shugaku_girl2_res_aux;
extern u8  cw97_cel_mode;
extern u8  cal_cw_flag4;
extern u8  cw97_prio;
extern u8  cal_cw_prio;
extern Blk68 cw98_active;
extern Blk68 cw96_active;
extern s16 cw98_frame;
extern u8  cursor_work_bak[];
extern s32 etc_rec_tbl0;
extern CLASS_W class_w[];
extern s32 shugaku_serifuev_tbl[];
extern s32 shugaku_serifuev_kind_tbl[];
extern s32 shugaku_soundev_tbl[];

extern void shugaku_bank_ptr_init2(void);
extern void shugaku_desc_init(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void make_color_bar16(u8 *a0, s32 a1, s32 a2, s32 a3, s32 arg4);
extern void make_fan(s32 a0, s32 a1, s32 a2);
extern void sub_sub_step_up(void);

void shugaku_girl2_event_init(void)
{
    s16 i;

    now_girl = 2;
    shugaku_float_on = 0;
    shugaku_float_cnt = 0;
    menu_x = 1;
    shugaku_bank_ptr_init2();
    with_yoshi = 0;
    move_in = 0;
    shugaku_ss_serifuev = shugaku_serifuev_tbl[19];
    shugaku_ss_serifuev_kind = shugaku_serifuev_kind_tbl[19];
    shugaku_sss_soundev = shugaku_soundev_tbl[19];
    shugaku_desc_init();
    palette_upload_enqueue(shugaku_girl2_res_pal, 17, 1, 2, 0);
    date_sprite_on = 0;
    wink2_stop = 0;
    bustup_mode = 0;
    for (i = 0; i < 6; i++) {
        *(s16 *)&Sprites[i * 36 + 2096] = 0;
    }
    date_object_set(shugaku_girl2_res_img, shugaku_girl2_res_anim_tbl, shugaku_girl2_res, shugaku_girl2_res_aux);
    cw97_cel_mode = 8;
    cal_cw_flag4 = 8;
    cw97_prio = 11;
    cal_cw_prio = 11;
    cw98_active = cw96_active;
    class_w[98].anim_flag = 64;
    cw98_frame = 20;
    make_color_bar16(cursor_work_bak, 0xFFFFFF, 0xFFFFFF, 16, 0);
    palette_upload_enqueue((s32)cursor_work_bak, 31, 1, 3, 0);
    etc_rec_tbl0 = 1;
    make_fan(0x801C4000, 37, 128);
    sub_sub_step_up();
}
