/* @SymbolSize: 368 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=56,8 */
/* shugaku_girl5_event_init - SHUGAKU overlay, Address: 0x80134648, Size: 368 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern void shugaku_bank_ptr_init2(void);
extern void shugaku_tbl_init3(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void param_clamp(void);
extern void strcpy(u8 *a0, u8 *a1);
extern void sub_sub_step_up(void);

extern s32 shugaku_ss_serifuev;
extern s32 shugaku_ss_serifuev_kind;
extern s32 shugaku_sss_soundev;
extern s32 shugaku_girl5_res_pal;
extern s32 shugaku_girl5_res_bg0;
extern s32 shugaku_girl5_res_bg1;
extern s32 shugaku_girl5_res_bg2;
extern s32 shugaku_girl5_res_bg3;
extern s32 shugaku_girl5_res_bg4;
extern s32 shugaku_girl5_res_bg5;
extern s32 shugaku_girl5_res;
extern s32 shugaku_girl5_res_cels;
extern s32 shugaku_girl5_res_anim_tbl;
extern s16 shugaku_girl5_res_scale;
extern volatile u8 now_girl;
extern u8 cal_cw_flag4;
extern s16 with_yoshi;
extern s16 move_in;
extern u8 screen_w[];
extern u8 ba_string[];
extern u8 shugaku_girl5_event_init_str0[];
extern CLASS_W class_w[];
extern s32 shugaku_serifuev_tbl[];
extern s32 shugaku_serifuev_kind_tbl[];
extern s32 shugaku_soundev_tbl[];

void shugaku_girl5_event_init(void)
{
    u8 *q;
    s32 eight;

    shugaku_bank_ptr_init2();
    shugaku_ss_serifuev = shugaku_serifuev_tbl[48];
    shugaku_ss_serifuev_kind = shugaku_serifuev_kind_tbl[48];
    shugaku_sss_soundev = shugaku_soundev_tbl[48];
    shugaku_tbl_init3();

    palette_upload_enqueue(shugaku_girl5_res_pal, 17, 1, 2, 0);
    date_sprite_init(shugaku_girl5_res_bg0, shugaku_girl5_res_bg1, shugaku_girl5_res_bg2, shugaku_girl5_res_bg3,
                  shugaku_girl5_res_bg4, shugaku_girl5_res_bg5);
    date_object_set(shugaku_girl5_res_cels, shugaku_girl5_res_anim_tbl, shugaku_girl5_res, (s32)shugaku_girl5_res_scale);

    eight = 8;
    cal_cw_flag4 = (u8)eight;
    with_yoshi = 0;
    move_in = 0;
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 446) = (s16)(*(s16 *)(q + 446) + 1);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 450) = (s16)(*(s16 *)(q + 450) + 2);
    class_w[97].cel_mode = (u8)eight;
    param_clamp();
    strcpy(ba_string, shugaku_girl5_event_init_str0);
    sub_sub_step_up();
}
