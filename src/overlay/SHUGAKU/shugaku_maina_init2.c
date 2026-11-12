/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xc8 */
/* @SymbolSize: 476 */
#include "types.h"
#include "tokimeki/class_w.h"

extern volatile s32 shugaku_serifuev_tbl_66;
extern volatile s32 shugaku_serifuev_kind_tbl_66;
extern volatile s32 shugaku_soundev_tbl_66;
extern volatile s32 shugaku_ss_serifuev;
extern volatile s32 shugaku_ss_serifuev_kind;
extern volatile s32 shugaku_sss_soundev;
extern volatile s32 shugaku_maina_res_pal;
extern volatile s32 shugaku_maina_res_spr0;
extern volatile s32 shugaku_maina_res_spr1;
extern volatile s32 shugaku_maina_res_spr2;
extern volatile s32 shugaku_maina_res_spr3;
extern volatile s32 shugaku_maina_res_spr4;
extern volatile s32 shugaku_maina_res_spr5;
extern volatile s32 shugaku_maina_res_obj_a2;
extern volatile s32 shugaku_maina_res_obj_a0;
extern volatile s32 shugaku_maina_res_obj_a1;
extern volatile s16 shugaku_maina_res_obj_scale;
extern volatile s16 with_yoshi;
extern volatile s16 move_in;
extern volatile s32 tpage_buf_5;
extern volatile s32 tpage_buf_6;
extern volatile s32 tpage_buf_7;
extern volatile s32 tpage_buf_8;
extern volatile s32 tpage_buf_9;
extern volatile s32 wink2_stop;
extern u8  voice_level;
extern u8  shugaku_voice_save;
extern u8  cal_cw_flag4;
extern volatile s16 cw97_frame;
extern volatile s16 cw98_frame;

extern void shugaku_bank_ptr_init2(void);
extern void shugaku_ptr_init(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void sub_sub_step_up(void);

typedef struct { s32 w[17]; } Blk68;
extern CLASS_W class_w[];

void shugaku_maina_init2(void)
{
    shugaku_bank_ptr_init2();
    shugaku_ss_serifuev = shugaku_serifuev_tbl_66;
    shugaku_ss_serifuev_kind = shugaku_serifuev_kind_tbl_66;
    shugaku_sss_soundev = shugaku_soundev_tbl_66;
    shugaku_ptr_init();
    palette_upload_enqueue(shugaku_maina_res_pal, 17, 1, 2, 0);
    date_sprite_init(shugaku_maina_res_spr0, shugaku_maina_res_spr1, shugaku_maina_res_spr2, shugaku_maina_res_spr3,
                  shugaku_maina_res_spr4, shugaku_maina_res_spr5);
    date_object_set(shugaku_maina_res_obj_a0, shugaku_maina_res_obj_a1, shugaku_maina_res_obj_a2, (s32)shugaku_maina_res_obj_scale);

    class_w[97].cel_mode = 8;
    cal_cw_flag4 = 8;
    cw97_frame = 3;

    (*(Blk68 *)&class_w[98]) = (*(Blk68 *)&class_w[96]);

    class_w[98].anim_flag = 64;
    cw98_frame = 40;
    class_w[98].prio = (u8)(class_w[98].prio - 1);
    with_yoshi = 3;
    move_in = 0;
    tpage_buf_5 = -1;
    tpage_buf_6 = -1;
    tpage_buf_7 = -1;
    tpage_buf_8 = -1;
    tpage_buf_9 = -1;
    wink2_stop = 1;
    shugaku_voice_save = voice_level;
    sub_sub_step_up();
}
