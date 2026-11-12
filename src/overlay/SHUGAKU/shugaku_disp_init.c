/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xd8 */
/* @SymbolSize: 340 */
#include "types.h"
#include "tokimeki/class_w.h"

extern volatile s32 shugaku_serifuev_tbl_29;
extern volatile s32 shugaku_serifuev_kind_tbl_29;
extern volatile s32 shugaku_soundev_tbl_29;
extern volatile s32 shugaku_ss_serifuev;
extern volatile s32 shugaku_ss_serifuev_kind;
extern volatile s32 shugaku_sss_soundev;
extern volatile s32 shugaku_girl3_res_pal;
extern volatile s32 shugaku_girl3_res_bg0;
extern volatile s32 shugaku_girl3_res_bg1;
extern volatile s32 shugaku_girl3_res_bg2;
extern volatile s32 shugaku_girl3_res_bg3;
extern volatile s32 shugaku_girl3_res_bg4;
extern volatile s32 shugaku_girl3_res_bg5;
extern volatile s32 shugaku_girl3_res;
extern volatile s32 shugaku_girl3_res_cels;
extern volatile s32 shugaku_girl3_res_anim_tbl;
extern volatile s16 shugaku_girl3_res_scale;
extern volatile s16 with_yoshi;
extern volatile s16 move_in;
extern u8  cal_cw_flag4;
extern volatile s16 cw98_anim;
extern CLASS_W class_w[];

extern void shugaku_bank_ptr_init2(void);
extern void shugaku_tbl_init2(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void sub_sub_step_up(void);

typedef struct { s32 w[17]; } Blk68;

void shugaku_disp_init(void)
{
    shugaku_bank_ptr_init2();
    shugaku_ss_serifuev = shugaku_serifuev_tbl_29;
    shugaku_ss_serifuev_kind = shugaku_serifuev_kind_tbl_29;
    shugaku_sss_soundev = shugaku_soundev_tbl_29;
    with_yoshi = 0;
    move_in = 0;
    shugaku_tbl_init2();

    palette_upload_enqueue(shugaku_girl3_res_pal, 17, 1, 2, 0);
    date_sprite_init(shugaku_girl3_res_bg0, shugaku_girl3_res_bg1, shugaku_girl3_res_bg2, shugaku_girl3_res_bg3,
                  shugaku_girl3_res_bg4, shugaku_girl3_res_bg5);
    date_object_set(shugaku_girl3_res_cels, shugaku_girl3_res_anim_tbl, shugaku_girl3_res, (s32)shugaku_girl3_res_scale);

    class_w[97].cel_mode = 8;
    cal_cw_flag4 = 8;

    (*(Blk68 *)&class_w[98]) = (*(Blk68 *)&class_w[96]);

    cw98_anim = 4;
    sub_sub_step_up();
}
