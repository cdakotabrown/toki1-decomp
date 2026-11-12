/* @CompileEnv: IDO52_UGEN_TEMP_REG=11:t1 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"

extern volatile s32 shugaku_girl2_res_pal;
extern volatile s16 shugaku_girl2_res_aux;
extern volatile s32 shugaku_girl2_res;
extern volatile s32 shugaku_girl2_res_anim_tbl;
extern volatile s32 shugaku_girl2_res_img;
extern u8  cal_cw_flag4;
extern u8  cw97_prio;
extern u8  cal_cw_prio;
extern volatile s16 cw98_frame;
extern volatile s32 etc_rec_tbl0;

extern u8 cursor_work_bak[];
extern u8 D_800EFD9C_b[];

typedef struct { u8 pad[36]; } Ent36;
extern Ent36 Sprites[];

extern void shugaku_desc_init(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void make_color_bar16(s32 a0, s32 a1, s32 a2, s32 a3, ...);
extern void make_fan(s32 a0, s32 a1, s32 a2);
extern void sub_sub_step_up(void);

typedef struct { s32 w[17]; } Blk68;
extern CLASS_W class_w[];

void shugaku_init3e(void)
{
    s32 i;

    shugaku_desc_init();
    palette_upload_enqueue(shugaku_girl2_res_pal, 17, 1, 2, 0);

    for (i = 0; i < 6; i = (s16)(i + 1)) {
        *(s16 *)((u8 *)&Sprites[i] + 0x830) = 0;
    }

    date_object_set(shugaku_girl2_res_img, shugaku_girl2_res_anim_tbl, shugaku_girl2_res, (s32)shugaku_girl2_res_aux);

    class_w[97].cel_mode = 8;
    cal_cw_flag4 = 8;
    cw97_prio = 11;
    cal_cw_prio = 11;

    (*(Blk68 *)&class_w[98]) = (*(Blk68 *)&class_w[96]);

    class_w[98].anim_flag = 64;
    cw98_frame = 20;
    make_color_bar16((s32)cursor_work_bak, 0xFFFFFF, 0xFFFFFF, 16, 0);
    palette_upload_enqueue((s32)D_800EFD9C_b, 31, 1, 3, 0);
    etc_rec_tbl0 = 1;
    make_fan(0x801C4000, 37, 128);
    sub_sub_step_up();
}
